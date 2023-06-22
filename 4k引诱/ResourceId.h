#pragma once
#include <Windows.h>

namespace BrushId {
	enum BrushId {
		Gray,
		Total
	};
}

namespace ImageId {
	enum ImageId {
		Line,
		Total
	};
}

namespace MessageId {
	enum MessageId {
		Update = WM_USER + 1
	};
}

namespace KeyId {
	enum KeyId {
		D = 0x44,
		F = 0x46,
		J = 0x4A,
		K = 0x4B,
		Enter = VK_RETURN,
		Up = VK_UP,
		Down = VK_DOWN
	};
}