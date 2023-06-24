
cmrc_add_resource_library(science_CL ALIAS amrc::science_cl
    gravpotential.cl
    colorizer.frag
    fixed.vert
)

set_property(TARGET science_CL PROPERTY
    POSITION_INDEPENDENT_CODE ON
)
