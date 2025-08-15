PORTNAME=	zfs
PORTVERSION=	1.0.0
CATEGORIES=	sysutils pear

MAINTAINER=	ports@FreeBSD.org
COMMENT=	PECL extension for OpenZFS on FreeBSD
WWW=		https://pacyworld.dev/penitentuary/pecl-freebsd-zfs

LICENSE=	PHP301

USES=		php:pecl

CPPFLAGS+=	-I${LOCALBASE}/include \
		-I/usr/src/cddl/compat/opensolaris/include \
		-I/usr/src/sys/contrib/openzfs/include \
		-I/usr/src/sys/contrib/openzfs/lib/libspl/include \
		-I/usr/src/sys/contrib/openzfs/lib/libspl/include/os/freebsd

LDFLAGS+=	-L${LOCALBASE}/lib ${EXTRA_LIBS}

CONFIGURE_ARGS=	--with-zfs

post-patch:
	@${REINPLACE_CMD} -e 's|\$$ZFS_DIR/lib|\$$ZFS_DIR/\$$PHP_LIBDIR|g' ${WRKSRC}/config.m4

.include <bsd.port.mk>
