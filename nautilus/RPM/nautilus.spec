
Name        	: nautilus
Version     	: 1.8rc3
Release     	: 1
Group       	: Multimedia/Audio
Summary     	: secure dialup/IP voice phone	
License			: proprietary / free for non commercial use
Packager    	: nautilus Secure Phone Maintainers http://nautilus.berlios.de 
Source        	: http://prdownload.berlios.de/nautilus/%{name}-%{version}.tar.gz
BuildRoot   	: /tmp/%{name}-%{version}

%define _topdir    /tmp/%{name}-%{version}-build
%define SHAREDIR /usr/share/nautilus-securephone

%Description
Nautilus allows half-duplex encrypted-voice conversation over standard 
phone lines or network links.  It requires a PC or Sun Sparcstation with 
speakers and microphone.  Additionally, a 4800 bps or faster modem or 
IP network connection to another computer running nautilus is required.

------
Authors: William Dorsey, Andrew Fingerhut, et al.
%Prep
###%setup -n %{name}-%{Version}
%setup -n %{name}-%{version}
echo 'LOGON_FILE = "%{SHAREDIR}/logon_new.v"' >>nautilus.cfg
echo 'RING_FILE = "%{SHAREDIR}/ring_new.v"'   >>nautilus.cfg

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

