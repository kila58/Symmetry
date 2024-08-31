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

#include "console_entry.h"
#include "console_manager.h"

#include "template_types.h"

#include "html_colours.h"

using namespace Lib;

NAMESPACE_CORE(Console)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SymConVarBase
///
/// \brief	Console variable base.  This can be used to represent a convar without knowing what
/// 		type it holds.  The base provides or ensures the ConVar provides two functions.
/// 		SetValueEx and GetValueEx, SetValue takes a string and GetValue returns a string,
/// 		that should both represent a string-like representation of the value it holds or
/// 		should hold.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SymConVarBase : public IConsoleEntry
{
public:

	virtual bool	SetValueEx(string){ return false; };
	virtual string	GetValueEx(){ return ""; };

	virtual string	GetDefaultValueEx(){ return ""; };

	virtual string	GetTypeName(){ return ""; };
	virtual int		GetType(){ return -1; };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct CVCallback
///
/// \brief Workaround for callbacks.
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct CVCallback
{
	typedef void(*type)(SymConVarBase*, T, T);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class SymConVar
///
/// \brief Console variable.  Provides a variable that can be modified in the console.
/// 	   T is the type it holds, most variables that can be represented as text can 
/// 	   be used in this.  Const variables are not supported.
///
/// \tparam T Generic type parameter.
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class SymConVar : public SymConVarBase
{
private:
	
	// Values
	T								m_Value, m_DefaultValue;

	// Called when the value's changed
	typename CVCallback<T>::type	m_cvCallback;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void SymConVar::SetDefaultValue(T value)
	///
	/// \brief	Sets the convar's default value.
	///
	/// \param	value	The default value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void	SetDefaultValue(T value)
	{
		this->m_DefaultValue = value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	T SymConVar::GetDefaultValue()
	///
	/// \brief	Returns the default value for this convar, returned type will be the same type as
	/// 		denoted by the template.
	///
	/// \return	The default value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	T		GetDefaultValue()
	{
		return this->m_DefaultValue;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void SymConVar::SetValue(T value)
	///
	/// \brief	Sets the value of this convar and calls the change-callback if one's set.
	///
	/// \param	value	The value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void	SetValue(T value)
	{
		T old = this->m_Value;
		this->m_Value = value;
		
		if (this->m_cvCallback != NULL)
			this->m_cvCallback(this, old, value);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual bool SymConVar::SetValueEx(string sValue)
	///
	/// \brief	Casts sValue to whatever* type this ConVar holds, then calls ConVar::SetValue on the
	/// 		casted value.  * Nothing stops this convar from holding a value that cannot be casted
	/// 		from a string. (e.g a class).  This function should return false if the value this
	/// 		convar holds cannot be casted.
	///
	/// \param	sValue	String-representation of the value.
	///
	/// \return	true if it succeeds, false if it fails.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool SetValueEx(string sValue)
	{
		stringstream ss(sValue);

		switch (TypeInfo<T>::iType)
		{
		case TYPE_UINT16:
		case TYPE_UINT32:
		case TYPE_UINT64:
		{	
							// GHETTO FIX FOR COLOURS
							string str = ss.str();
							uint32 len = str.length();

							// Make sure it's full of 0's (0x00000000)
							ss << hex << setfill('0') << setw(8);
							
							if (len > 0)
							{
								// Remove hashtag thing, used for colours
								if (str[0] == '#')
								{
									// Remove a single byte
									char c; ss >> c;

									// Remove from string too, for html colours
									str = str.substr(1, string::npos);
								}

								// Check if yo nigger string is a preprocessor value
								for (uint32 i = 0; i < GetHtmlColourCount(); i++)
								{
									HtmlColour col = g_pHtmlColours[i];

									string lowercase = str;
									transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

									if (col.Name == lowercase)
									{
										ss.str("");
										ss << setfill('0') << setw(8) << col.HexName;

										break;
									}
								}
							}
		}
		case TYPE_INT16:
		case TYPE_INT32:
		case TYPE_INT64:
		{
						   string str = ss.str();
						   uint32 len = str.length();

						   // Starts with 0x (0xFFFFFFFF)
						   if (len > 2 && str[0] == '0' && str[1] == 'x')
						   {
							   // Remove two bytes ('0' and 'x')
							   char a, b;
							   ss >> a >> b;
							   ss >> hex;
						   }
		}
		case TYPE_DOUBLE:
		case TYPE_FLOAT:
		{
							 T inst; 
							 if (ss >> inst)
							 {
								 this->SetValue(inst);
							 }
							 else
								 // Conversion failed, return false on that bitch
								 return false;
							 

							 break;
		}
		case TYPE_STRING:
		case TYPE_STRINGP:
		{
							 string buf;
							 getline(ss, buf);

							 this->SetValue(*(T*)&buf);
		}
		case TYPE_UNKNOWN:
			return false;
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual string SymConVar::GetValueEx()
	///
	/// \brief	Gets the value of this convar, representing it as a string.
	///
	/// \return	The value of this convar.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual string	GetValueEx()
	{
		stringstream ss; 
		switch (TypeInfo<T>::iType)
		{
		case TYPE_UINT16:
		case TYPE_UINT32:
		case TYPE_UINT64:
			ss << "#" << setfill('0') << setw(9) << uppercase << hex << this->m_Value;
			break;
		default:
			ss << this->m_Value;
		}

		return ss.str();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual string SymConVar::GetDefaultValueEx()
	///
	/// \brief	Gets the default value of this convar, representing it as a string.
	///
	/// \return	The default value this convar.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual string	GetDefaultValueEx()
	{
		stringstream ss;
		switch (TypeInfo<T>::iType)
		{
		case TYPE_UINT16:
		case TYPE_UINT32:
		case TYPE_UINT64:
			ss << "#" << uppercase << hex << this->m_Value;
			break;
		default:
			ss << this->m_DefaultValue;
		}

		return ss.str();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual string SymConVar::GetTypeName()
	///
	/// \brief	Returns the name of the type this convar holds.
	///
	/// \return	The type name.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual string GetTypeName()
	{
		return TypeNames[ this->GetType() ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual int SymConVar::GetType()
	///
	/// \brief	Returns the typeid of the type this convar holds.
	///
	/// \return	The typeid.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual int GetType()
	{
		return TypeInfo<T>::iType;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	T SymConVar::GetValue()
	///
	/// \brief	Returns this convar's value.
	///
	/// \return	The value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	T		GetValue()
	{
		return this->m_Value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void SymConVar::SetCallback(typename CVCallback<T>::type cvCallback)
	///
	/// \brief	Self-explanatory.
	///
	/// \param	cvCallback	The convar callback.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void	SetCallback(typename CVCallback<T>::type cvCallback)
	{
		this->m_cvCallback = cvCallback;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConVar::SymConVar(string sName, T value)
	///
	/// \brief	Constructor.
	///
	/// \param	sName	The name of the convar.
	/// \param	value	The default value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConVar(string sName, T value)
	{
		this->m_sName			= sName;
		this->m_Value			= value;
		this->m_DefaultValue	= value;
		this->m_cvCallback		= NULL;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConVar::SymConVar(string sName, string sHelpString, T value)
	///
	/// \brief	Constructor.
	///
	/// \param	sName	   	The name of the convar.
	/// \param	sHelpString	The help string.
	/// \param	value	   	The default value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConVar(string sName, string sHelpString, T value)
	{
		this->m_sName			= sName;
		this->m_Value			= value;
		this->m_DefaultValue	= value;
		this->m_sHelpString		= sHelpString;
		this->m_cvCallback		= NULL;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConVar::SymConVar(string sName, string sHelpString, T value,
	/// 	typename CVCallback<T>::type cvCallback)
	///
	/// \brief	Constructor.
	///
	/// \param	sName	   	The name of the convar.
	/// \param	sHelpString	The help string.
	/// \param	value	   	The default value.
	/// \param	cvCallback 	The convar-changed callback, this callback will be called when the convar
	/// 					is changed.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConVar(string sName, string sHelpString, T value, typename CVCallback<T>::type cvCallback)
	{
		this->m_sName			= sName;
		this->m_Value			= value;
		this->m_DefaultValue	= value;
		this->m_sHelpString		= sHelpString;

		this->SetCallback(cvCallback);

		ConsoleManager::RegistryEntryProxy(this);
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////
///								    == MACROS / DEFINITIONS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MakeConVar5(type, name, help, value, callback)\
	SymConVar<type> ##name("" #name "", help, value, callback)

#define MakeConVar4(type, name, help, value)\
	SymConVar<type> ##name(string("" #name ""), string(help), value)

#define MakeConVar3(type, name, value)\
	SymConVar<type> ##name(string("" #name ""), value)

#define IntegerToColor(integer)\
	Color((##integer & 0xFF000000) >> 24, (##integer & 0x00FF0000) >> 16, \
	(##integer & 0x0000FF00) >> 8, (##integer & 0x000000FF))

#define ConvarToColor(convar) IntegerToColor(##convar.GetValue())

NAMESPACE_END2