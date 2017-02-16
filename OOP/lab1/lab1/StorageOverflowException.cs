using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    enum StorageOverflowExcArg {
        LIGHT_OVERFLOW,
        LARGE_OVERFLOW
    }

    class StorageOverflowException : ApplicationException
    {
        public StorageOverflowException() { }

        public StorageOverflowException(StorageOverflowExcArg arg, string message) : base(message)
        {
            Console.WriteLine("StorageOverflowExcArg! Arg: " + arg);
        }

        public StorageOverflowException(string message, Exception inner) : base(message, inner) { }
    }
}
