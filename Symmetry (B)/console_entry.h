/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

NAMESPACE_CORE(Console)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class IConsoleEntry
///
/// \brief ConVars and ConCommands should inherit this.  It provides basic methods and
/// 	   an ability to represent both in the same linked list.  There's no point defining
/// 	   this in a source file.
////////////////////////////////////////////////////////////////////////////////////////////////////

class IConsoleEntry
{
protected:

	// Name of the convar/concommand
	string				m_sName;

	// Help string
	string				m_sHelpString;

	// Aliases
	vector<string>		m_vAliases;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn virtual string IConsoleEntry::GetName()
	///
	/// \brief Returns the name of this console entry.
	///
	/// \return The name of this console entry.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual string GetName()
	{
		return this->m_sName;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn virtual string IConsoleEntry::GetHelp()
	///
	/// \brief Returns the help string associated with this instance.
	///
	/// \return The help string.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual string GetHelp()
	{
		return this->m_sHelpString;
	}
};

NAMESPACE_END2