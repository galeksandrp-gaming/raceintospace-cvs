# Configure paths for libtheora
# Karl Heyes 02-Feb-2004

dnl XIPH_PATH_THEORA([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl Test for libtheora, and define THEORA_CFLAGS THEORA_LIBS
dnl

AC_DEFUN([XIPH_PATH_THEORA],
[
XIPH_PATH_OGG([$1],[$2])

dnl Get the cflags and libraries for theora
dnl
AC_ARG_VAR([THEORA],[path to theora installation])
AC_ARG_WITH(theora,
    AC_HELP_STRING([--with-theora=PREFIX],
        [Prefix where libtheora is installed (optional)]),
    theora_prefix="$withval",
    theora_prefix="$THEORA_PREFIX"
    )
if test "x$theora_prefix" = "x"; then
    if test "x$prefix" = "xNONE"; then
        theora_prefix="/usr"
    else
        theora_prefix="$prefix"
    fi
fi

THEORA_CFLAGS="$OGG_CFLAGS"
THEORA_LDFLAGS="$OGG_LDFLAGS"
if test "x$theora_prefix" != "x$ogg_prefix"; then
        THEORA_CFLAGS="$THEORA_CFLAGS -I$theora_prefix/include"
        THEORA_LDFLAGS="-L$theora_prefix/lib $THEORA_LDFLAGS"
fi

THEORA_LIBS="-ltheora"

ac_save_LIBS="$LIBS"
ac_save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $THEORA_LDFLAGS"
LIBS="$LIBS $THEORA_LIBS"
xt_have_theora="yes"
AC_MSG_CHECKING([for Theora])
AC_TRY_LINK_FUNC(ogg_stream_init,,
        [LIBS="$LIBS $OGG_LIBS"
        AC_TRY_LINK_FUNC(ogg_stream_init,
            [THEORA_LIBS="$THEORA_LIBS $OGG_LIBS"],
            [xt_have_theora="no"])
        ])

LIBS="$ac_save_LIBS"
LDFLAGS="$ac_save_LDFLAGS"

if test "x$xt_have_theora" = "xyes"
then
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_THEORA],[1],[Define if Theora support is available])
else
    ifelse([$2], , AC_MSG_ERROR([Unable to link to libtheora]), [$2])
    THEORA_CFLAGS=""
    THEORA_LDFLAGS=""
    THEORA_LIBS=""
fi
AC_SUBST(THEORA_CFLAGS)
AC_SUBST(THEORA_LDFLAGS)
AC_SUBST(THEORA_LIBS)
])
