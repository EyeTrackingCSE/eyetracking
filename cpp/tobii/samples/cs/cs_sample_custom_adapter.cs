/*
COPYRIGHT 2018-2020  - PROPERTY OF TOBII AB
-------------------------------------
2018-2020 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law.
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
permission is obtained from Tobii AB.
*/

using System;
using System.Runtime.InteropServices;

namespace CSharpSampleCustomAdapter
{
    // A helper wrapper class for creating and destroying an instance of the native IL_AdapterFuncs structure.
    public class AdapterFuncs : IDisposable
    {
        private const String dllName = "CustomAdapter";

        [DllImport(dllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "CreateCustomAdapter")]
        private static extern IntPtr CreateCustomAdapter();

        [DllImport(dllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DestroyCustomAdapter")]
        private static extern void DestroyCustomAdapter(IntPtr adapterFuncs);

        public IntPtr Handle { get; } = CreateCustomAdapter();

        #region IDisposable Support

        private bool _isDisposed = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_isDisposed)
            {
                DestroyCustomAdapter(Handle);
                _isDisposed = true;
            }
        }

        ~AdapterFuncs()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion
    }

    class Program
    {
        static void Main(string[] args)
        {
            // create the native custom adapter
            var adapter = new AdapterFuncs();

            // create the interaction library
            Tobii.InteractionLib.IInteractionLib intlib =
                Tobii.InteractionLib.InteractionLibFactory.CreateInteractionLib(
                    Tobii.InteractionLib.FieldOfUse.Interactive, adapter.Handle);

            Console.WriteLine("Starting interaction library update loop.");

            // setup and maintain device connection, wait for device data between events and 
            // update interaction library to trigger all callbacks, stop after 10 cycles
            const int max_cycles = 10;
            var cycle = 0;

            while (cycle++ < max_cycles)
            {
                intlib.WaitAndUpdate();
            }

            // cleanup - here done explicitly with Dispose(). The alternative is to use a using statement,
            // which implicitly disposes an instance when the scope is left.
            intlib.Dispose();
            adapter.Dispose();
        }
    }
}
