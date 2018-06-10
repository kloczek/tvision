
info_TEXINFOS 	= tvision.texi

EXTRA_DIST	= \
	Doxyfile \
	example.gif.in \
	index.html.in

SRCS = app.h buffers.h colorsel.h dialogs.h drawbuf.h editors.h \
	help.h helpbase.h menus.h msgbox.h objects.h outline.h resource.h \
	stddlg.h system.h textview.h tkeys.h tobjstrm.h ttypes.h tv.h \
	tvconfig.h tvobjs.h util.h validate.h views.h

myclean:
	rm tvision.info
	rm -fr html

mydoc: tvision.info buildhtml

tvision.info:
	makeinfo tvision.texi

buildhtml:
	@if test -d html; then \
	  echo "Removing old html directory"; \
	  rm -fr html; \
	fi; \
	mkdir html

	@echo "Generating installation handbook..."; \
	texi2html tvision.texi; \
	mv tvision.html html

	@echo "Generating documentation for class hierarchy"; \
	IFS="${IFS=	}"; save_IFS="$IFS"; IFS=":"; \
	for dir in $$PATH; do \
	  echo "Searching in $$dir"; \
	  for prog in doxygen; do \
	    if test -x "$$dir/$$prog"; then \
	      doxy="$$dir/$$prog"; \
	      break 2; \
	    fi \
	  done \
	done; \
	IFS="$$save_IFS"; \
	if test -n "$$doxy"; then \
	  echo "Found $$doxy, please wait..."; \
	  $$doxy; \
	else \
	  echo "Didn't find doxygen in your path, can't build documentation"; \
	fi

	@echo "Creating file index.html..."; \
	cp index.html.in html/index.html; \
	cp example.gif.in html/example.gif
