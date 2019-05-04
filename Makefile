MBEDTLS = $(ROOT_DIR)/mbedtls

CFLAGS = -fPIC -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -O2
LDFLAGS =

INCLUDES = -I$(MBEDTLS)/include

LIBS = mbedtls/library/libmbedx509.a mbedtls/library/libmbedtls.a mbedtls/library/libmbedcrypto.a

SOURCES = https.c

OBJS = $(SOURCES:.c=.o)


compile:
	gcc -o main $(CFLAGS) -Imbedtls/include main.c TaskVariable.c TextProcessing.c MathEquationsParser.c ApiService.c preg_replace.c https.c Memd/mem.c tinyexpr.c cJSON.c -lpcre2-8  `pkg-config --cflags --libs webkitgtk-3.0 libnotify` libmemd.a $(LIBS) -lm -lncurses