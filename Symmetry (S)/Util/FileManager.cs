/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.Console;
using Symmetry.Util.Crypto;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Symmetry.Util
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \class  SvnFile
    ///
    /// \brief  A file from the SVN repository that's fully accessible to Symmetry clients.  The file
    ///         REQUIRES a info file in the same directory, such that.  File.a has a File.info alongside
    ///         it.  No exceptions are caught in this classes's constructor, any exceptions that are
    ///         thrown should mark the file as inaccessible.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class SvnFile
    {
        public int Branch;

        public bool IsMapped;       // True if the file should be mapped into the game
        public bool IsEncrypted;    // True if the file should be encrypted with AES before being sent to the client
        public bool IsLuaAutorun;   // True if the Lua file should be automatically ran by the injected binary
        public bool IsInjectorFile; // True if the file belongs to the injector

        public byte[] Data;

        public string Path;

        public SvnFile(string path, string infoPath)
        {
            if (!File.Exists(path))
                throw new IOException("File not found.");

            if (!File.Exists(infoPath))
                throw new IOException("Information file not found.");

            XmlDocument doc = new XmlDocument();
            doc.Load(infoPath);

            XmlNodeList branch = doc.GetElementsByTagName("Branch");
            XmlNodeList mapped = doc.GetElementsByTagName("IsMapped");
            XmlNodeList encrypted = doc.GetElementsByTagName("IsEncrypted");
            XmlNodeList autorun = doc.GetElementsByTagName("IsLuaAutorun");
            XmlNodeList injectorfile = doc.GetElementsByTagName("IsInjectorFile");

            this.Branch = FileManager.BranchNameToId(branch.Count != 0 ? branch[0].InnerText : "beta");
            this.IsMapped = mapped.Count != 0 ? Convert.ToBoolean(mapped[0].InnerText) : false;
            this.IsEncrypted = encrypted.Count != 0 ? Convert.ToBoolean(encrypted[0].InnerText) : true;
            this.IsLuaAutorun = autorun.Count != 0 ? Convert.ToBoolean(autorun[0].InnerText) : false;
            this.IsInjectorFile = injectorfile.Count != 0 ? Convert.ToBoolean(injectorfile[0].InnerText) : false;

            this.Data = File.ReadAllBytes(path);
            this.Path = path;
        }
    }

    class FileManager
    {

        // Static dictionary containing cached files for each branch.
        // Files are cached when they're requested and refreshed every minute.  (A request to get files
        // will re-cache files if the last cache was made more than a minute ago)
        static Dictionary<int, List<SvnFile>> FileCache = new Dictionary<int, List<SvnFile>>();

        // Times that files were cached
        static Dictionary<int, DateTime> FileCacheTimes = new Dictionary<int, DateTime>();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn static int BranchNameToId(string branch)
        ///
        /// \brief  Converts a branch name to a integer-id.  Relatively generic code, but if I ever
        ///         need to add more branches having the conversion done in a function is nice.
        ///
        /// \param  branch  The name of the branch.
        ///
        /// \return The ID of the branch, -1 if the branch name does not exist.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static int BranchNameToId(string branch)
        {
            switch (branch)
            {
                case "public":
                    return 0;
                case "beta":
                    return 1;
            }

            return -1;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string BranchIdToString(int branch)
        ///
        /// \brief  Converts a branch integer-id to name.
        ///
        /// \param  branch  The branch's ID.
        ///
        /// \return The branch name as per the ID.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string BranchIdToString(int branch)
        {
            switch (branch)
            {
                case 0:
                    return "public";
                case 1:
                    return "beta";
            }

            return "";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static List<SvnFile> GetFilesInBranch(int branch)
        ///
        /// \brief  Returns a list of files in a branch.
        ///
        /// \param  branch  The branch to get a list of files for.
        ///
        /// \return The files in the branch.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static List<SvnFile> GetFilesInBranch(int branch)
        {
            // Check cache for cached files
            if (FileManager.FileCacheTimes.ContainsKey(branch))
            {
                DateTime time = FileManager.FileCacheTimes[branch];

                // Cache made within one minute
                if (time.AddMinutes(1) >= DateTime.Now)
                    return FileManager.FileCache[branch];
            }

            List<SvnFile> list = new List<SvnFile>();

            if (!Directory.Exists(Properties.Settings.Default.SvnPath))
                return list;

            // Don't include files that either don't include a . (no file extension)
            // or start with a . (svn uses them)
            string[] files = Directory.GetFiles(Properties.Settings.Default.SvnPath, "*", SearchOption.AllDirectories);

            // Include each file in list
            foreach (string file in files)
            {
                // Can't start with '.' and it can't not have a file extension
                if (file.StartsWith(".")) continue;
                if (!file.Contains(".")) continue;

                string infoFile = file + ".xml";

                // Each file REQUIRES an information file
                if (File.Exists(infoFile))
                {
                    // Xml needs to parse properly, if it doesn't
                    // or if any other errors occur, don't add the file to the
                    // list.
                    try
                    {
                        SvnFile sf = new SvnFile(file, infoFile);
                        if (sf.Branch == branch) list.Add(sf);
                    }
                    catch (Exception e)
                    {
                        Log.Print(LogFlags.ERROR, "Couldn't load information file {0}: {1}", infoFile, e.Message);
                    }
                }
            }

            // Cache files for later
            FileManager.FileCache[branch] = list;
            FileManager.FileCacheTimes[branch] = DateTime.Now;

            return list;
        }
    }
}
