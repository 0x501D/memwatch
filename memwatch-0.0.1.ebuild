# Copyright 1999-2015 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=5

DESCRIPTION="Prints memory status in real time"
HOMEPAGE="https//bitbucket.org/PascalRD/memwatch/"
SRC_URI="http://unixdev.ru/src/memwatch-0.0.1.tar.gz"

LICENSE="WTFPL"
SLOT="0"
KEYWORDS="amd64 x86 ppc sparc"
IUSE=""

DEPEND="sys-libs/ncurses"
RDEPEND="${DEPEND}"

src_prepare() {
	einfo "Regenerating autotools files..."
	./autogen.sh || die "autogen failed"
}

src_configure() {
	econf
}

src_compile() {
	emake
}

src_install() {
	emake DESTDIR="${D}" install
}
