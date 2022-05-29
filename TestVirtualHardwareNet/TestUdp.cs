using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VirtualHardwareNet;

namespace TestVirtualHardwareNet
{
    public class TestUdp
    {
        private EthernetNet _ethernet = new EthernetNet();
        private int _socketNumber = -1;

        public void Setup()
        {
            int result = _ethernet.udpBegin(8888, ref _socketNumber);

            for (int i = 0; i < 1000; i++) {
                Console.WriteLine(i);
                _ethernet.udpClose(_socketNumber);
                result = _ethernet.udpBegin(8888, ref _socketNumber);
            }
        }

        public void Loop()
        {
            uint remoteIpAddress;
            ushort remotePort;

            int size = _ethernet.udpParsePacket(_socketNumber, out remoteIpAddress, out remotePort);
            if (size > 0) {
                byte[] data = new byte[256];
                uint length = (uint)data.Length;
                int sizeRead = _ethernet.udpRead(_socketNumber, ref data, length);

                // Send echo back to client
                _ethernet.udpBeginPacket(_socketNumber, "127.0.0.1", remotePort);
                _ethernet.udpWrite(_socketNumber, data, (uint)sizeRead);
                _ethernet.udpEndPacket(_socketNumber);

                // Send echo also to port + 1 to check if implementation can handle sends to non existing clients
                // See issue: https://stackoverflow.com/a/39440399
                _ethernet.udpBeginPacket(_socketNumber, "127.0.0.1", (ushort)(remotePort + 1));
                _ethernet.udpWrite(_socketNumber, data, (uint)sizeRead);
                _ethernet.udpEndPacket(_socketNumber);
            }
        }

        public void End()
        {
            _ethernet.udpClose(_socketNumber);
        }
    }
}
