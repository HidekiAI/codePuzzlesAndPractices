using System;
// From Google Interview tip sheet:
// Hashtables: Arguably the single most important data structure know to mankind.  You absolutely shopuld
// know how they work.  Be able to implement one using only arraysin your favorite language.  In about the
// space of one interview.

namespace HashTableWithArray
{
    public class HashTable<T>
    {
        int count;  // for compartors and iterations
        int kBlockSize = 16; // dynamically grow array, when Insert is called, it is O(16 * blocks), not O(count), but by keeping blockSize small, O(n) should be neglegent
        int kFixedArraySize = 1024;
        // If Tuples are allowed as part of the primitives, I'd approah it as:
        (int Hash, T Value)[]   NamedTupleTable;
        // Or more descriptive:
        Tuple<int, T>[] TupleTable;
        // If Tuples isnot allowed:
        int[] HashTable;
        T[] Values;
        // if doing Fixed array size (Brute force):
        Tuple<int, T>[] FixedSizeTable; // this is the fastest method, since you just do modulus operation of hash % kFixedArraySize!

        public HashTable(int blockSize)
        {
            var newBlockSize = blockSize + (blockSize % kBlockSize);    // just to be safe in case they pass blockSize < kBlockSize

            // if doing named tuple method:
            NamedTupleTable = new (int, T)[newBlockSize];
            // if doing Tuple<> method:
            TupleTable = new Tuple<int, T>[newBlockSize];
            // if using strictly primitives arrayed:
            HashTable = new int[newBlockSize];
            Values = new T[newBlockSize];
            // if doing fixed Array size that do not grow:
            FixedSizeTable = new Tuple<int,T>[kFixedArraySize]();


            count = 0;
        }

        public bool Insert(T value)
        {
            var inserted=false;
            if (count + 1 > HastTable.Count())
            {
                HashTable = HashTable.Concat(new int[kBlockSize]).ToArray(); // concat make it into IEnumerable, convert back to Array
                Values = Values.Concat(new T[kBlockSize]).ToArray();
            }
            var hash = value.GetHash();
            // O(n) search
            for(i = 0; i < count + 1); ++i)
            {
                if (HashTable[i] == hash)
                {
                    throw new Exception("Hash collsion");
                }
                else if (Values[i] == default(T))
                {
                    // O(n) if no slots found until end of the table, we can iterate reverse order if table is huge
                    HashTable[i] = hash;
                    Values[i] = value;
                    ++count;
                    break;  // opt out early
                }
            }
            // alternatively, if we made hash to be the index (brute force)
            if (FixedSizeTable[hash % kFixedArraySize] == hash)
            {
                throw new Exception("Hash collsion");
            }
            else
            {
                FixedSizeTable[hash % kFixedArraySize] = value;
            }
        }
    }

    public static class Program
    {

        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }
    }
}
