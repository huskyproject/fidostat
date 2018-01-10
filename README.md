# Hello!

This is **fidostat**, a quick hack of a log file analyser for the binkd TCP/IP
FTN mailer. It is intended to be used for seeing which systems have polled (or
have been polled by) the user's system today. For example:

```
$ fidostat binkdstat
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
FidoStat/lnx 1.9.0-cur 09-01-18 Binkd Poll Statistic

   Date: Wed Jan 10 11:13:43 2018
   System: 2:292/789.0, Niels Joncheere

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  4 Sessions with 2:203/0@fidonet, 78-66-161-203-no236.tbcn.telia.com [78.66.161.203] (2996)
  1 Sessions with 2:240/1120@fidonet, p5B01FACE.dip0.t-ipconnect.de [91.1.250.206] (24554)
 12 Sessions with 2:291/111@fidonet, cl.fidonode.be [77.243.226.167] (2807)
  1 Sessions with 2:292/624@fidonet, rasp04.02.dco.fusa.be [185.18.148.59] (24554)
  9 Sessions with 2:292/854@fidonet, 133.151-243-81.adsl-dyn.isp.belgacom.be [81.243.151.133] (62626)
  8 Sessions with 2:292/8125@fidonet, cust-194-216-110-94.dyn.as47377.net [94.110.216.194] (1564)
```

Such reports are sent to standard output by default, but can easily be
redirected to a file or piped to another program in order to, e.g., post the
report in your local statistics echo. For example:

```bash
# Redirect to file and post to local echo:
fidostat binkdstat > stats.txt
hpt post -s "Daily binkd statistics" -e "MILLENNIUM_STATS" -x stats.txt
```

Alternatively:

```bash
# Post to local echo via pipe:
fidostat binkdstat | hpt post -s "Daily binkd statistics" -e "MILLENNIUM_STATS" -x -
```

The programm can't do much at the time of writing, but perhaps it will be
a universal tool one day :)

The logfile binkd.cfg is searched in your standart fidoconfig logfile
directory.

Have phun,
     Gabriel Plutzar

gabriel@hit.priv.at, 2:31/1
