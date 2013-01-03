#include "Router.h"

#include "ssu/UDPTransport.h"

#include "handlers/DatabaseStore.h"

#include "util/Base64.h"

namespace i2pcpp {
	void Router::start()
	{
		m_inMsgDispatcher.registerHandler(I2NP::Message::Type::DB_STORE, MessageHandlerPtr(new Handlers::DatabaseStore(m_ctx)));

		m_transport = TransportPtr(new SSU::UDPTransport(m_ctx));

		std::shared_ptr<SSU::UDPTransport> u = std::dynamic_pointer_cast<SSU::UDPTransport>(m_transport);
		u->begin(Endpoint("127.0.0.1", 27333));

		m_transport->connect(Base64::decode("zhPja0k1cboGnHbhqO50hNPTVHIRE8b4GMwi7Htey~E="));

		m_jobRunnerPool.push_back(JobRunnerPtr(new JobRunner(m_jobQueue)));
		for(auto jr: m_jobRunnerPool)
			jr->start();
	}

	void Router::stop()
	{
		std::shared_ptr<SSU::UDPTransport> u = std::dynamic_pointer_cast<SSU::UDPTransport>(m_transport);
		u->shutdown();

		for(auto jr: m_jobRunnerPool)
			jr->stop();
	}
}
