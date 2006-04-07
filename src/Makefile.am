bin_PROGRAMS = amethyst


# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
amethyst_LDFLAGS = $(all_libraries) 
lib_LTLIBRARIES = libamethyst.la
libamethyst_la_LDFLAGS = -module
libamethyst_la_SOURCES = engine.cpp object.cpp physics.cpp utility.cpp \
		vector.cpp engine.h object.h physics.h utility.h vector.h orientation.cpp \
		orientation.h net.cpp net.h
amethyst_LDADD = $(top_builddir)/src/libamethyst.la
amethyst_SOURCES = console.cpp console_history.cpp console_io.cpp \
	console_menu.cpp console_menu_main.cpp main.cpp test.cpp
amethyst_DEPENDENCIES = libamethyst.la
amethystincludedir = $(includedir)/amethyst
amethystinclude_HEADERS = engine.h object.h physics.h utility.h vector.h \
			version.h orientation.h net.h

noinst_HEADERS = net.h
