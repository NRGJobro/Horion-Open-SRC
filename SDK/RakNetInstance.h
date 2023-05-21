#pragma once

class RakNetInstance {
	char pad_0000[0x3C8];  // 0x0000
public:
	TextHolder numericalIp;  // 0x03D0
	TextHolder serverIp;
	uint32_t serverPort;

private:
	virtual ~RakNetInstance();
public:
	virtual TextHolder* getLocalIps(TextHolder*);
	virtual TextHolder* getLocalIp(TextHolder*);
	virtual __int64 getPort(void);
	virtual __int64 getRefinedLocalIps(void);
	virtual int getConnectedGameInfo(void);
	virtual void setupNatPunch(bool);
	virtual int getNatPunchInfo(void);
	virtual void startNatPunchingClient(std::string const &, short);
	virtual void addConnectionStateListener(__int64*);
	virtual void removeConnectionStateListener(__int64*);
	virtual bool isIPv4Supported(void);
	virtual bool isIPv6Supported(void);
	virtual int getIPv4Port(void);
	virtual int getIPv6Port(void);
	virtual void host(__int64 const &);
	virtual void connect(void *);
	virtual void disconnect(void);
	virtual void tick(void);
	virtual void runEvents(void);
	virtual bool isServer(void);
	virtual int getTransportIdentifier(void);
	virtual int getPeer(void);
	virtual int getPeer1(void);
	virtual void _onDisable(void);
	virtual void _onEnable(void);

public:
	bool isonaServer() { return !(serverIp.getTextLength() < 1); }

};                         //Size: 0x0848