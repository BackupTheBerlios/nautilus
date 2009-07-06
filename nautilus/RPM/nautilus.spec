
Name        	: nautilus
Version     	: 1.8rc2
Release     	: 1
Group       	: Multimedia/Audio
Summary     	: secure dialup/IP voice phone
Copyright   	: 1999 Ed Casas; GPL
Packager    	: nautilus Secure Phone Maintainers http://nautilus.berlios.de 
Source        	: http://www.franken.de/crypt/nautilus/nautilus-1.8rc2.tar.gz
Patch0		: nautilus-1.8rc2.sounddir.patch
BuildRoot   	: /tmp/%{name}-%{version}

%define SHAREDIR /usr/share/nautilus-securephone
%Description
Nautilus allows half-duplex encrypted-voice conversation over standard 
phone lines or network links.  It requires a PC or Sun Sparcstation with 
speakers and microphone.  Additionally, a 4800 bps or faster modem or 
IP network connection to another computer running nautilus is required.

%Prep
###%setup -n %{name}-%{Version}
%setup -n %{name}-%{version}
%patch0

%Build
# Force use of safer macros
#%{fixUP} -g conf_script groff 'groff -S'
( cd rsaref; make CC="gcc $RPM_OPT_FLAGS" -f makefile.unx )
make CC="gcc $RPM_OPT_FLAGS" linux
strip nautilus

%Install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
DESTDIR=$RPM_BUILD_ROOT
export DESTDIR

install -d $DESTDIR/usr/bin
install -d $DESTDIR%{_mandir}/man1
install -d $DESTDIR/etc
install -d $DESTDIR%{SHAREDIR}
install -m644 nautilus.cfg $DESTDIR/etc/nautilus.cfg
install -m644 logon_new.v $DESTDIR%{SHAREDIR}
install -m644 ring_new.v $DESTDIR%{SHAREDIR}
make BINDIR=$DESTDIR/usr/bin MANDIR=$DESTDIR/%{_mandir} \
	SHAREDIR=$DESTDIR%{SHAREDIR} install

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%doc Changelog doc/*
%{_mandir}/man1/*.1*
%{SHAREDIR}/*
%config /etc/*
/usr/bin/*

