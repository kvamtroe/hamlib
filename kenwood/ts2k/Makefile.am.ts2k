TSSRCLIST = ts850.c ts870s.c ts570.c ts450s.c ts950.c ts50s.c \
		ts440.c ts790.c thd7.c thf7.c ts2000.c k2.c

lib_LTLIBRARIES = hamlib-kenwood.la
hamlib_kenwood_la_SOURCES = $(TSSRCLIST) kenwood.c th.c \
		ts2k/ts2k.c ts2k/ts2k_menu.c ts2k/ts2k_mdata.c ts2k/ts2k_cmds.c
hamlib_kenwood_la_LDFLAGS = -no-undefined -module -avoid-version
hamlib_kenwood_la_LIBADD = $(top_builddir)/src/libhamlib.la @MATH_LIBS@

noinst_HEADERS = kenwood.h th.h ts2k/ts2k.h ts2k/ts2k_menu.h ts2k/ts2k_cmds.h
