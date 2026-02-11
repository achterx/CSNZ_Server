// CORRECT FIX - Replace lines 233-254 with this:

		// when a people may incorrect once packet data, might spammed this message forever....
		m_pMsg = new CReceivePacket(Buffer(packetDataBuf));
		if (!m_pMsg->IsValid())
		{
			// DEBUG: Log what we actually received
			Logger().Error("CExtendedSocket::Read(%s): received invalid packet\n", GetIP().c_str());
			if (packetDataBuf.size() > 0)
			{
				Logger().Info("[DEBUG] Packet size: %d bytes\n", (int)packetDataBuf.size());
				
				// Build hex string
				std::string hexStr = "[DEBUG] First 20 bytes (hex): ";
				size_t maxBytes = packetDataBuf.size() < 20 ? packetDataBuf.size() : 20;
				for (size_t i = 0; i < maxBytes; i++)
				{
					char buf[8];
					sprintf(buf, "%02X ", (unsigned char)packetDataBuf[i]);
					hexStr += buf;
				}
				Logger().Info("%s\n", hexStr.c_str());
				
				Logger().Info("[DEBUG] Expected signature: 0x%02X ('U'), Got: 0x%02X ('%c')\n", 
				             TCP_PACKET_SIGNATURE, (unsigned char)packetDataBuf[0], 
				             (packetDataBuf[0] >= 32 && packetDataBuf[0] < 127) ? packetDataBuf[0] : '?');
			}
			delete m_pMsg;
			m_pMsg = NULL;
			return NULL;
		}

		if (m_pMsg->GetSequence() != m_nNextExpectedSeq)
		{
			Logger().Error("CExtendedSocket::Read(%s): sequence mismatch, got: %d, expected: %d\n", GetIP().c_str(), m_pMsg->GetSequence(), m_nNextExpectedSeq);
			delete m_pMsg;
			m_pMsg = NULL;
			return NULL;
		}
