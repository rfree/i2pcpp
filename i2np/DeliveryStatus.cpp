#include "DeliveryStatus.h"

namespace i2pcpp {
	namespace I2NP {
		bool DeliveryStatus::parse(ByteArray::const_iterator &dataItr)
		{
			m_msgId = (*(dataItr++) << 24) | (*(dataItr++) << 16) | (*(dataItr++) << 8) | *(dataItr++);
			m_timestamp = Date(dataItr);

			return true;
		}

		ByteArray DeliveryStatus::getBytes() const
		{
		}
	}
}
