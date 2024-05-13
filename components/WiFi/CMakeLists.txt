set(component_srcs)

idf_component_register(SRCS "wifi.c" "DFPlayer.c" "${component_srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS ".")
