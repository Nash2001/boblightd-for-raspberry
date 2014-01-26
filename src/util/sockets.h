/*
 * boblight
 * Copyright (C) Bob  2009 
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOCKET
#define SOCKET

#include <string>
#include <netinet/in.h>
#include <vector>

#define FAIL    0
#define SUCCESS 1
#define TIMEOUT 2

#define SERVER_PATH "/tmp/boblightd.socket"

class CData
{
  public:
    void SetData(uint8_t* data, int size, bool append = false);
    /*void SetData(char* data, int size, bool append = false);
    void SetData(const uint8_t* data, int size, bool append = false);
    void SetData(const char* data, int size, bool append = false);*/
    void SetData(std::string data, bool append = false);

    int   GetSize() { return m_data.size() - 1; }
    char* GetData() { return &m_data[0]; }
                                                                                          
    void Clear();

  private:
    std::vector<char> m_data;
    void CopyData(char* data, int size, bool append);
};

class CSocket //base class
{
  public:
    CSocket();
    ~CSocket();

    virtual int TcpOpen(std::string address, int port, int usectimeout = -1);
    virtual int UnixOpen(std::string address, int port, int usectimeout = -1);
    
    void Close();
    bool IsOpen() { return m_sock != -1; }

    std::string GetError()   { return m_error; }
    std::string GetAddress() { return m_address; }
    int         GetPort()    { return m_port; }
    int         GetSock()    { return m_sock; }

    void        SetTimeout(int usectimeout) { m_usectimeout = usectimeout; }
    
  protected:
    std::string m_address;
    std::string m_error;

    int     m_sock;
    int     m_usectimeout;
    int     m_port;

    int SetNonBlock(bool nonblock = true);
    int SetSockOptions();
    int SetKeepalive();
    int WaitForSocket(bool write, std::string timeoutstr);
};

class CClientSocket : public CSocket
{
  public:
    int TcpOpen(std::string address, int port, int usectimeout = -1);
    int UnixOpen(std::string address, int port, int usectimeout = -1);
    
    int Read(CData& data);
    int Write(CData& data);
    int SetInfo(std::string address, int port, int sock);

};

class CServerSocket : public CSocket
{
  public:
    int TcpOpen(std::string address, int port, int usectimeout = -1); //empty string for interface means bind to *
    int TcpAccept(CClientSocket& socket);
    
    int UnixOpen(std::string address, int port, int usectimeout = -1); //empty string for interface means bind to *
    int UnixAccept(CClientSocket& socket);
};
#endif //SOCKET
