using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

internal static class StartTestClient
{
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
    private static extern uint GetPrivateProfileString(
        string section, string key, string defaultValue, StringBuilder value, int capacity, string path);

    private static string ReadServerSetting(string path, string key)
    {
        var value = new StringBuilder(256);
        uint length = GetPrivateProfileString("server", key, "", value, value.Capacity, path);
        if (length == 0 || length >= value.Capacity - 1)
            throw new FormatException("请在 config.ini 的 [server] 中填写 " + key + "。");
        return value.ToString();
    }

    [STAThread]
    private static int Main(string[] args)
    {
        bool checkOnly = args.Length == 1 && args[0] == "--check";
        if (args.Length != 0 && !checkOnly)
            return 2;

        try
        {
            string root = AppDomain.CurrentDomain.BaseDirectory;
            string config = Path.Combine(root, "config.ini");
            string client = Path.Combine(root, "Data", "zhengtu.dat");
            if (!File.Exists(client))
                throw new FileNotFoundException("找不到 Data\\zhengtu.dat。请把启动器放在游戏主目录。");

            IPAddress address;
            if (!IPAddress.TryParse(ReadServerSetting(config, "loginaddress"), out address) ||
                address.AddressFamily != AddressFamily.InterNetwork || address.Equals(IPAddress.Any))
                throw new FormatException("loginaddress 必须是测试服务器的 IPv4 地址。");

            ushort port;
            ushort zone;
            if (!ushort.TryParse(ReadServerSetting(config, "loginport"), NumberStyles.None, CultureInfo.InvariantCulture, out port) || port == 0)
                throw new FormatException("loginport 必须是 1 到 65535 的整数。");
            if (!ushort.TryParse(ReadServerSetting(config, "zone"), NumberStyles.None, CultureInfo.InvariantCulture, out zone) || zone == 0)
                throw new FormatException("zone 必须是 1 到 65535 的整数。");

            var start = new ProcessStartInfo
            {
                FileName = client,
                WorkingDirectory = root,
                Arguments = string.Format(CultureInfo.InvariantCulture, "-runzt {0} {1} {2}", address, port, zone),
                UseShellExecute = false
            };

            if (checkOnly)
            {
                Console.WriteLine("Client: {0}", start.FileName);
                Console.WriteLine("Arguments: {0}", start.Arguments);
                Console.WriteLine("Working directory: {0}", start.WorkingDirectory);
                return 0;
            }

            using (Process process = Process.Start(start))
            {
                if (process.WaitForExit(2000))
                {
                    string message = string.Format(CultureInfo.InvariantCulture,
                        "客户端启动后立即退出（退出码 0x{0:X8}）。", process.ExitCode);
                    MessageBox.Show(message, "征途测试服", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return 1;
                }
                return 0;
            }
        }
        catch (Exception error)
        {
            if (!(error is IOException || error is FormatException || error is Win32Exception))
                throw;
            if (checkOnly)
                Console.Error.WriteLine(error.Message);
            else
                MessageBox.Show(error.Message, "征途测试服", MessageBoxButtons.OK, MessageBoxIcon.Error);
            return 1;
        }
    }
}
