# redirect to subdirectories
SUBDIRS = src inc examples config

# install the restframes-config script
bin_SCRIPTS = restframes-config

# install the restframes-config script
libexec_SCRIPTS = setup_RestFrames.sh setup_RestFrames.csh

CLEANFILES = $(bin_SCRIPTS) $(libexec_SCRIPTS)

# do_subst = $(SED) \
# 	-e 's|[@]bindir@|$(bindir)|g' \
# 	-e 's|[@]libdir@|$(libdir)|g' \
# 	-e 's|[@]includedir@|$(includedir)|g' \
# 	-e 's|[@]prefix@|$(prefix)|g' \
# 	-e 's|[@]CONFIG_CXXFLAGS@|$(CONFIG_CXXFLAGS)|g' \
# 	-e 's|[@]CONFIG_LIBS@|$(CONFIG_LIBS)|g' \
# 	-e 's|[@]CONFIGURE_INVOCATION@|$(CONFIGURE_INVOCATION)|g' \
# 	-e 's|[@]PACKAGE_NAME@|$(PACKAGE_NAME)|g' \
# 	-e 's|[@]VERSION@|$(VERSION)|g' \
# 	-e 's|\n|\\n|g' \
# 	< "$<" > "$@"

do_subst = $(SED) \
	-e 's|[@]prefix@|$(prefix)|g' \
	-e 's|[@]bindir@|$(bindir)|g' \
	-e 's|[@]libdir@|$(libdir)|g' \
	-e 's|[@]includedir@|$(includedir)|g' \
	-e 's|\n|\\n|g' \
	< "$<" > "$@"

EXTRA_DIST = $(top_srcdir)/config/restframes-config.in\
	$(top_srcdir)/config/setup_RestFrames.sh.in\
	$(top_srcdir)/config/setup_RestFrames.csh.in

# restframes-config: $(top_srcdir)/config/restframes-config.in Makefile
# 	$(do_subst) < $(top_srcdir)/config/restframes-config.in > restframes-config
# 	chmod u+x restframes-config

setup_RestFrames.sh: $(top_srcdir)/config/setup_RestFrames.sh.in Makefile
	$(do_subst) < $(top_srcdir)/config/setup_RestFrames.sh.in > setup_RestFrames.sh
	chmod u+x setup_RestFrames.sh

setup_RestFrames.csh: $(top_srcdir)/config/setup_RestFrames.csh.in Makefile
	$(do_subst) < $(top_srcdir)/config/setup_RestFrames.csh.in > setup_RestFrames.csh
	chmod u+x setup_RestFrames.csh

distclean-local: 
	rm -f inc/RestFrames/RestFrames_config.h

install-data-hook:
	@echo
	@echo "   ============================================================"
	@echo "                RestFrames INSTALLATION SUCCESSFUL!            "
	@echo "   ============================================================"
	@echo
	@echo "                 INFORMATION FOR RestFrames USERS              "
	@echo "   ---   "
	@echo
	@echo "    When using the RestFrames library, you can retrieve"
	@echo "    compilation and link flags using restframes-config"
	@echo "    (located in the 'bin' directory). "
	@echo "    Type 'restframes-config --help' for more information"
	@echo
	@echo "   ---   "
	@echo
	@echo "    The script setup_RestFrames.sh(csh) is provided to"
	@echo "    automatically set environemtal variables like PATH,"
	@echo "    LD_LIBRARY_PATH, etc. in order to find this RestFrames"
	@echo "    installation. Sourcing this script (from any directory) will"
	@echo "    set the environmental variables to the values matching the"
	@echo "    RestFrames build that created the script (located in the"
	@echo "    libexec directory)."
	@echo
	@echo "   ============================================================"
	@echo

