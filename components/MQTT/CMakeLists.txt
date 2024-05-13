set(component_srcs)

idf_component_register(SRCS "mqtt.c" "DFPlayer.c" "${component_srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS ".")
