#include "Router.h"

/* TEMPORARY */
#include "util/Base64.h"
#include "i2np/DatabaseLookup.h"
#include "i2np/DatabaseStore.h"
#include "ssu/UDPTransport.h"

namespace i2pcpp {
	Router::~Router()
	{
		m_serviceThread.join();
	}

	void Router::start()
	{
		m_serviceThread = std::thread([&](){m_ios.run();});

		m_transport = TransportPtr(new SSU::UDPTransport(m_ctx));
		m_transport->registerReceivedHandler(boost::bind(&InboundMessageDispatcher::messageReceived, m_inMsgDispatcher, _1, _2));
		m_transport->registerEstablishedHandler(boost::bind(&InboundMessageDispatcher::connectionEstablished, m_inMsgDispatcher, _1));
		m_outMsgDispatcher.registerTransport(m_transport);

		std::shared_ptr<SSU::UDPTransport> u = std::dynamic_pointer_cast<SSU::UDPTransport>(m_transport);
		u->start(Endpoint("127.0.0.1", 27333)); // TODO Config setting
		u->connect(Base64::decode("zhPja0k1cboGnHbhqO50hNPTVHIRE8b4GMwi7Htey~E="));
	}

	void Router::stop()
	{
		m_ios.stop();
	}

	void Router::databaseLookup(std::string const &to, std::string const &query)
	{
		ByteArray queryBytes = Base64::decode(query);
		std::array<unsigned char, 32> key;
		std::copy(queryBytes.cbegin(), queryBytes.cbegin() + 32, key.begin());

		RouterHash toHash = Base64::decode(to);
		I2NP::MessagePtr dbl(new I2NP::DatabaseLookup(key, m_ctx.getMyRouterHash(), 0));

		m_ctx.getOutMsgDispatcher().sendMessage(toHash, dbl);
	}

	void Router::createTunnel(std::string const &to)
	{
		/*std::list<BuildRequestRecord>;
			I2NP::MessagePtr vtb(new I2NP::VariableTunnelBuild(hopList));*/
	}
}
