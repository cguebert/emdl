#include <emdl/VR.h>
#include <emdl/Exception.h>

#include <boost/container/flat_map.hpp>
#include <vector>

namespace
{
	struct VRInfo
	{
		emdl::VR vr;
		std::string name;
		emdl::VRType type;
	};

	using VRInfos = std::vector<VRInfo>;
	const VRInfos& getVRInfos()
	{
		// clang-format off
		using VRT = emdl::VRType;
		static VRInfos vrInfos = {
			{ emdl::VR::Unknown, "",   VRT::Binary,	},
			{ emdl::VR::AE,      "AE", VRT::String,	},
			{ emdl::VR::AS,      "AS", VRT::String,	},
			{ emdl::VR::AT,      "AT", VRT::String,	},
			{ emdl::VR::CS,      "CS", VRT::String,	},
			{ emdl::VR::DA,      "DA", VRT::String,	},
			{ emdl::VR::DS,      "DS", VRT::Real,	},
			{ emdl::VR::DT,      "DT", VRT::String,	},
			{ emdl::VR::FL,      "FL", VRT::Real,	},
			{ emdl::VR::FD,      "FD", VRT::Real,	},
			{ emdl::VR::IS,      "IS", VRT::Int,	},
			{ emdl::VR::LO,      "LO", VRT::String,	},
			{ emdl::VR::LT,      "LT", VRT::String,	},
			{ emdl::VR::OB,      "OB", VRT::Binary,	},
			{ emdl::VR::OD,      "OD", VRT::Binary,	},
			{ emdl::VR::OF,      "OF", VRT::Binary,	},
			{ emdl::VR::OL,      "OL", VRT::Binary,	},
			{ emdl::VR::OW,      "OW", VRT::Binary,	},
			{ emdl::VR::PN,      "PN", VRT::String,	},
			{ emdl::VR::SH,      "SH", VRT::String,	},
			{ emdl::VR::SL,      "SL", VRT::Int,	},
			{ emdl::VR::SQ,      "SQ", VRT::Dataset,},
			{ emdl::VR::SS,      "SS", VRT::Int,	},
			{ emdl::VR::ST,      "ST", VRT::String,	},
			{ emdl::VR::TM,      "TM", VRT::String,	},
			{ emdl::VR::UC,      "UC", VRT::String,	},
			{ emdl::VR::UI,      "UI", VRT::String,	},
			{ emdl::VR::UL,      "UL", VRT::Int,	},
			{ emdl::VR::UN,      "UN", VRT::Binary,	},
			{ emdl::VR::UR,      "UR", VRT::String,	},
			{ emdl::VR::US,      "US", VRT::Int,	},
			{ emdl::VR::UT,      "UT", VRT::String,	},
			{ emdl::VR::Invalid, "",   VRT::Binary  }
		};
		// clang-format on

		return vrInfos;
	}

	using VRMap = boost::container::flat_map<std::string, emdl::VR>;
	const VRMap& getVRMap()
	{
		static VRMap vrMap = [] {
			VRMap vrm;
			const auto& vri = getVRInfos();
			vrm.reserve(vri.size() - 2);
			for (const auto& i : vri)
			{
				if (!i.name.empty())
					vrm[i.name] = i.vr;
			}
			return vrm;
		}();

		return vrMap;
	}
}

namespace emdl
{
	std::string asString(VR vr)
	{
		if (vr == VR::Unknown || vr == VR::Invalid)
			throw Exception("Invalid VR");
		return getVRInfos()[static_cast<int>(vr)].name;
	}

	VR asVr(const std::string& vr)
	{
		const auto& vrm = getVRMap();
		const auto it = vrm.find(vr);
		if (it != vrm.end())
			return it->second;
		return VR::Unknown;
	}

	VRType vrType(VR vr)
	{
		return getVRInfos()[static_cast<int>(vr)].type;
	}
}
