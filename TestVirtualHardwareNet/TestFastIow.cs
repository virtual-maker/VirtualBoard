using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VirtualHardwareNet;

namespace TestVirtualHardwareNet
{
    public class TestFastIow
    {
        private GPIONet gpio;

        public void Setup()
        {
            gpio = new GPIONet();

            gpio.Begin(null);
            //gpio.Begin("100041F0");

            gpio.PinMode(13, 0);

            return;

            var spi = new SpiNet();
            spi.Begin();

            spi.BeginTransaction(100000, 0, 0);

            byte[] tData = new byte[2] { 0x00, 0x00 };
            byte[] rData = new byte[2];

            spi.Transfer(tData, ref rData, 2);

            spi.End();

            var i2c = new TwiNet();
            i2c.Begin();

            byte txAddress = 0x20;
            byte[] txBuffer = new byte[] { 6, 0x00, 0x00 };
            byte txBufferLength = (byte)txBuffer.Length;
            bool sendStop = true;

            var ret = i2c.WriteTo(txAddress, txBuffer, txBufferLength, true, sendStop);

            i2c.End();
        }

        public void End()
        {
            gpio.End();
        }

        public void Loop()
        {
            gpio.DigitalWrite(13, 1);
            Thread.Sleep(500);

            gpio.DigitalWrite(13, 0);
            Thread.Sleep(500);
        }
    }
}
