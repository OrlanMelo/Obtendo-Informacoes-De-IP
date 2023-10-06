#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib,"ws2_32.lib")

using namespace std;

class cFuncoes
{
private:

	PMIB_TCPTABLE TabelaIPV4;
	PMIB_TCP6TABLE TabelaIPV6;

	struct in_addr EnderecoIPV4;
	//struct in_addr6 EnderecoIPV6;

public:

	void ObterInformacoesDeIPV4()
	{
		ULONG Res;

		TabelaIPV4 = (MIB_TCPTABLE*)malloc(sizeof(PMIB_TCPTABLE));

		ULONG Bytes = 0;

		GetTcpTable(TabelaIPV4, &Bytes, true);
		free(TabelaIPV4);

		TabelaIPV4 = (MIB_TCPTABLE*)malloc(Bytes);

		Res = GetTcpTable(TabelaIPV4, &Bytes, true);
		if (Res == NO_ERROR)
		{			
			for (int i = 0; i < TabelaIPV4->dwNumEntries; i++)
			{
				EnderecoIPV4.S_un.S_addr = (u_long)TabelaIPV4->table[i].dwLocalAddr;
				cout << "Endereço IPV4 local: " << inet_ntoa(EnderecoIPV4) << '\n';
				EnderecoIPV4.S_un.S_addr = (u_long)TabelaIPV4->table[i].dwLocalPort;
				cout << "Endereço de porta local: " << inet_ntoa(EnderecoIPV4) << '\n';
				EnderecoIPV4.S_un.S_addr = (u_long)TabelaIPV4->table[i].dwRemoteAddr;
				cout << "Endereço remoto IPV4: " << inet_ntoa(EnderecoIPV4) << '\n';
				EnderecoIPV4.S_un.S_addr = (u_long)TabelaIPV4->table[i].dwRemotePort;
				cout << "Porta remota: " << inet_ntoa(EnderecoIPV4) << '\n';

				switch (TabelaIPV4->table[i].State)
				{
				case MIB_TCP_STATE_CLOSED:
					cout << "Status IPV4: Conexão fechada";
					break;
				case MIB_TCP_STATE_LISTEN:
					cout << "Status IPV4: Aguardando solicitação de conexão";
					break;
				case MIB_TCP_STATE_ESTAB:
					cout << "Status IPV4: Conexão aberta, estabelecida";
					break;
				case MIB_TCP_STATE_CLOSING:
					cout << "Status IPV4: Aguardando solicitação de encerramento";
					break;
				case MIB_TCP_STATE_CLOSE_WAIT:
					cout << "Status IPV4: Aguardando solicitação de encerramento do usuário";
					break;
				default:
					break;
				}

				cout << '\n';
				cout << '\n';
			}
		}
		else
		{
			cout << "Não foi possível obter informações de tabela em IPV4." << GetLastError();
		}

		if (TabelaIPV4)
			free(TabelaIPV4);

	}
	void ObterInformacoesDeIPV6()
	{
		ULONG Res;

		ULONG Bytes = 0;

		TabelaIPV6 = (MIB_TCP6TABLE*)malloc(sizeof(PMIB_TCP6TABLE));

		GetTcp6Table(TabelaIPV6, &Bytes, true);
		free(TabelaIPV6);

		TabelaIPV6 = (MIB_TCP6TABLE*)malloc(Bytes);

		Res = GetTcp6Table(TabelaIPV6, &Bytes, true);
		if (Res == NO_ERROR)
		{
			for (int i = 0; i < TabelaIPV6->dwNumEntries; i++)
			{
				char Ip[46];
				inet_ntop(AF_INET6, &TabelaIPV6->table[i].LocalAddr, Ip, 46);
				cout << "Endereço IPV6 local: " << Ip << '\n';
				cout << "Endereço porta local: " << ntohs((u_short)TabelaIPV6->table[i].dwLocalPort) << '\n';
				cout << "Endereço porta remota: " << ntohs((u_short)TabelaIPV6->table[i].dwRemotePort) << '\n';

				switch (TabelaIPV6->table[i].State)
				{
				case MIB_TCP_STATE_CLOSED:
					cout << "Status IPV6: Conexão fechada";
					break;
				case MIB_TCP_STATE_LISTEN:
					cout << "Status IPV6: Aguardando solicitação de conexão";
					break;
				case MIB_TCP_STATE_ESTAB:
					cout << "Status IPV6: Conexão aberta, estabelecida";
					break;
				case MIB_TCP_STATE_CLOSING:
					cout << "Status IPV6: Aguardando solicitação de encerramento";
					break;
				case MIB_TCP_STATE_CLOSE_WAIT:
					cout << "Status IPV6: Aguardando solicitação de encerramento do usuário";
					break;
				default:
					break;
				}

				cout << '\n';
				cout << '\n';
			}

			if (TabelaIPV6)
				free(TabelaIPV6);
		}
		else
		{
			cout << "Não foi possível obter informações de tabela em IPV6." << GetLastError();
		}
	}

}Funcoes;

int main()
{
	cout << "O assistente está verificando propriedades de IPV4 e/ou IPV6...";

	//Você poderá usar as duas ao mesmo tempo ou escolher a função que será necessária.
	Funcoes.ObterInformacoesDeIPV4();
	Funcoes.ObterInformacoesDeIPV6();

	system("pause");
}