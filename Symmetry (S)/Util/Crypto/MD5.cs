/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Util.Crypto
{
    class MD5
    {   
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string Compute(string input)
        ///
        /// \brief  Computes the MD5 hash of the input string.
        ///
        /// \param  input   The string to hash.
        ///
        /// \return The hash of the string.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string Compute(string input)
        {
            using (System.Security.Cryptography.MD5 md5 = System.Security.Cryptography.MD5.Create())
            {
                byte[] raw = md5.ComputeHash(Encoding.ASCII.GetBytes(input));
                return string.Concat(raw.Select(b => b.ToString("x2")).ToArray());
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string Compute(byte[] input)
        ///
        /// \brief  Overload for above, to take byte[] input.
        ///
        /// \param  input   The input.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string Compute(byte[] input)
        {
            return Compute(Encoding.ASCII.GetString(input));
        }
    }
}
