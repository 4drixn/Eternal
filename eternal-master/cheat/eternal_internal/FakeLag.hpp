#pragma once

namespace fakelag {

	inline int delay = 100;
	namespace inbound {
		inline bool m_enabled = false;
	}

	namespace outbound {
		inline bool m_enabled = false;
	}

	inline bool should_do_it = false;
}