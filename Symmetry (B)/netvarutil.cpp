/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "netvarutil.h"

#include "global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ClientClass* NetVarUtil::FindTable(string sName)
////////////////////////////////////////////////////////////////////////////////////////////////////

RecvTable* NetVarUtil::FindTable(string sName)
{
	if (!Src::Client)
		return NULL;

	ClientClass* current = Src::Client->GetAllClasses();

	// Iterate over linked list until we have a matching name
	for (; current; current = current->m_pNext)
	{
		if (sName == current->GetName())
		{
			return current->m_pRecvTable;
		}
		else if (RecvTable* tbl = NetVarUtil::FindSubTable(current->m_pRecvTable, sName))
			return tbl;
	}

	// No matching name
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RecvTable* NetVarUtil::FindSubTable(RecvTable* pTable, string sName)
////////////////////////////////////////////////////////////////////////////////////////////////////

RecvTable* NetVarUtil::FindSubTable(RecvTable* pTable, string sName)
{
	if (!pTable)
		return NULL;

	// This table matches our subtable search
	if (sName == pTable->GetName())
		return pTable;

	for (int i = 0; i < pTable->GetNumProps(); i++)
	{
		RecvProp* prop = pTable->GetPropA(i);

		if (prop)
		{
			if (RecvTable* tbl2 = NetVarUtil::FindSubTable(prop->GetDataTable(), sName))
				return tbl2;
		}

	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector<ClientClass*> NetVarUtil::FindClasses(string sName)
///
/// \brief	Searches for multiple matches for sName, sName can be a regular expression.
///
/// \param	sName	The name or regular expression.
///
/// \return	Class list.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<RecvTable*> NetVarUtil::FindTables(string sName)
{
	vector<RecvTable*> results;

	if (!Src::Client)
		return results;

	ClientClass* current = Src::Client->GetAllClasses();

	// Iterate over linked list added each matching class to
	// results
	for (; current; current = current->m_pNext)
	{
		if (Mem::IsRegexMatchSub(current->GetName(), sName))
			results.push_back(current->m_pRecvTable);
	}

	return results;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RecvProp* NetVarUtil::FindProp(RecvTable* pTable, string sName)
///
/// \brief	Searches for the first datatable prop inside a datatable pTable by name sName.
/// 		
/// 		This function will search recursively for props.
///
/// \param [in,out]	pTable	The datatable.
/// \param	sName		  	The name.
///
/// \return	null if it fails, else the found datatable prop.
////////////////////////////////////////////////////////////////////////////////////////////////////

RecvProp* NetVarUtil::FindProp(RecvTable* pTable, string sName)
{
	for (int i = 0; i < pTable->GetNumProps(); i++)
	{
		RecvProp* prop = pTable->GetPropA(i);

		// Matching prop?
		if (sName == prop->GetName())
			return prop;

		// Recursive check on subtables
		RecvTable* subTable = prop->GetDataTable();
		if (subTable && subTable != pTable)
		{
			if (RecvProp* prop2 = NetVarUtil::FindProp(subTable, sName))
				return prop2;
		}
	}

	// No matching prop was found
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector<RecvProp*> FindProps(RecvTable* pTable, string sName)
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<RecvProp*> NetVarUtil::FindProps(RecvTable* pTable, string sName)
{
	vector<RecvProp*> results;

	for (int i = 0; i < pTable->GetNumProps(); i++)
	{
		RecvProp* prop = pTable->GetPropA(i);

		// Matching prop?
		if (Mem::IsRegexMatchSub(prop->GetName(), sName))
			results.push_back(prop);

		// Recursive check on subtables
		RecvTable* subTable = prop->GetDataTable();
		if (subTable && subTable != pTable)
		{
			vector<RecvProp*> results2 = NetVarUtil::FindProps(subTable, sName);
			for (RecvProp* prop2 : results2)
				results.push_back(prop2);
		}
	}

	return results;
}
