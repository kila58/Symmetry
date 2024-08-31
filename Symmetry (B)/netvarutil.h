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

class NetVarUtil
{
public:

	static RecvTable*				FindTable(string sName);
	static vector<RecvTable*>		FindTables(string sName);
	static RecvTable*				FindSubTable(RecvTable* pClient, string sName);

	static RecvProp*				FindProp(RecvTable* pTable, string sName);
	static vector<RecvProp*>		FindProps(RecvTable* pTable, string sName);
};