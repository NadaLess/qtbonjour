/*
Copyright (c) 2007, Trenton Schulz
Copyright (c) 2009-2011, Stefan Hacker
Copyright (C) 2009-2011, Thorvald Natvig <thorvald@natvig.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

// Own includes
#include "bonjourrecord.h"

// libdnssd
#if defined(Q_OS_LINUX)
#include <avahi-compat-libdns_sd/dns_sd.h>
#else
#include <dns_sd.h>
#endif

// Qt includes
#include <QtCore/QMap>
#include <QtCore/QObject>

class QSocketNotifier;

namespace qtbonjour {
class BonjourServiceResolver : public QObject {
		Q_OBJECT
	protected:
		struct ResolveRecord {
			BonjourRecord record;
			BonjourServiceResolver *bsr;
			DNSServiceRef dnssref;
			QSocketNotifier *bonjourSocket;
			int bonjourPort;
			ResolveRecord(const BonjourRecord &, BonjourServiceResolver *);
			~ResolveRecord();
		};

		QMap<int, ResolveRecord *> qmResolvers;
	public:
                BonjourServiceResolver(QObject *parent = 0);
		~BonjourServiceResolver();

		void resolveBonjourRecord(const BonjourRecord &record);
	Q_SIGNALS:
		void bonjourRecordResolved(BonjourRecord record, QString hostname, int port);
		void error(BonjourRecord record, DNSServiceErrorType error);
	private Q_SLOTS:
		void bonjourSocketReadyRead(int);
	private:
		static void DNSSD_API bonjourResolveReply(DNSServiceRef sdRef, DNSServiceFlags flags,
		        quint32 interfaceIndex, DNSServiceErrorType errorCode,
		        const char *fullName, const char *hosttarget, quint16 port,
		        quint16 txtLen, const char *txtRecord, void *context);
};
} // qtbonjour
