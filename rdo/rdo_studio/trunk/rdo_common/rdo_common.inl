inline RDOColor::RDOColor()
	: transparent(true)
{}

inline RDOColor::RDOColor(rbyte _r, rbyte _g, rbyte _b, rbool _transparent)
	: r          (_r          )
	, g          (_g          )
	, b          (_b          )
	, transparent(_transparent)
{}

inline RDOColor::RDOColor(CREF(RDOColor) color)
	: r          (color.r          )
	, g          (color.g          )
	, b          (color.b          )
	, transparent(color.transparent)
{}
