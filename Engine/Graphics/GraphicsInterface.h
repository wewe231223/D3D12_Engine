#pragma once 

namespace EngineFramework {
	__interface GameObject {

	};

	__interface GameResource {
		virtual void Create() PURE;
	};
}