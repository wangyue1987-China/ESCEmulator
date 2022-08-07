#include <iostream>
#include <vector>
#include <string>
#include "EscManager.h"
#include "OSALAdapter.h"
int main()
{
	ESC::Esc esc1("Slave1","hjhj");
	//ESC::Esc esc2("Slave2");
	//ESC::Esc esc3("Slave3");

	OSAL::OSALAdapter adapter("eth0");
	adapter.AdapterOpen();
	ESC::EscManager escManager(adapter);
	escManager.addEsc(&esc1);
	/*escManager.addEsc(esc2);
	escManager.addEsc(esc3);*/
	escManager.start();
	while (1) {}
}
