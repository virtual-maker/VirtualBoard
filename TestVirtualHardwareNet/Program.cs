using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VirtualHardwareNet;

namespace TestVirtualHardwareNet
{
    class Program
    {
        private TestFastIow testFastIow = new TestFastIow();
        private TestUdp testUdp = new TestUdp();

        static void Main(string[] args)
        {
            var program = new Program();

            bool repeat = true;

            program.Setup();
            while (repeat)
            {
                program.Loop();
            }

            program.End();
        }

        public void End()
        {
            testFastIow.End();
            //testUdp.End();
        }

        public void Setup()
        {
            testFastIow.Setup();
            //testUdp.Setup();
        }

        public void Loop()
        {
            testFastIow.Loop();
            //testUdp.Loop();
        }
    }
}
