## Intel 2.66Ghz Core i7, i686-apple-darwin11-llvm-gcc-4.2 ##

```
Thu Feb 23 06:32:17 EST 2012

Message size = 20
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  0.03	  0.04	  1.06x		  0.06	  0.02	  0.03	  0.02	  0.03	
Decode	  0.02	  0.04	  1.91x		  0.04	  0.06	  0.03	  0.50	

Message size = 200
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  0.24	  0.26	  1.05x		  0.49	  0.20	  0.20	  0.22	  0.21	
Decode	  0.18	  0.46	  2.53x		  0.32	  0.28	  0.27	  5.22	

Message size = 2000
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  2.21	  2.43	  1.10x		  4.59	  1.95	  1.73	  2.06	  1.97	
Decode	  1.63	  4.35	  2.67x		  3.18	  2.60	  2.65	 51.78
```

## Intel(R) Xeon(R) CPU L5310  @ 1.60GHz ##

This is from a monster dual quad-core server.

```

gcc version 3.4.6 20060404 (Red Hat 3.4.6-3)
Thu Jul  5 16:53:46 UTC 2007

Message size = 20
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.05    0.06    1.20x           0.11    0.08    0.06    0.07    0.04
Decode    0.07    0.11    1.57x           0.08    0.05    0.10    0.87

Message size = 200
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.53    0.66    1.25x           1.19    0.41    0.48    0.62    0.39
Decode    0.41    0.92    2.24x           0.80    0.48    0.87    9.26

Message size = 2000
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    4.81    5.49    1.14x          10.55    3.82    4.82    5.91    3.79
Decode    3.93    8.94    2.27x           7.76    4.65    8.39   92.21

```

## 2.2Ghz AMD, gcc 3.4.4 -O3 ##

```
Thu Jul  5 17:01:59 UTC 2007
gcc version 3.4.6 20060404 (Red Hat 3.4.6-3)


Message size = 20
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.04    0.09    2.25x           0.09    0.06    0.04    0.10    0.06
Decode    0.04    0.13    3.25x           0.07    0.05    0.07    0.68

Message size = 200
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.41    0.91    2.22x           0.81    0.52    0.38    0.90    0.52
Decode    0.31    1.20    3.87x           0.55    0.43    0.56    6.71

Message size = 2000
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    3.91    8.71    2.23x           7.93    5.01    3.62    8.97    5.05
Decode    2.94   11.62    3.95x           5.44    4.17    5.39   66.84
```

## 2.2 Ghz Intel Core 2 Duo, gcc 4.0.1 -O3, Mac Os 10.5.1 ##

```
$ date
Thu Dec  6 10:00:41 EST 2007

$ ./speedtest

Message size = 20
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  0.07	  0.11	  1.53x		  0.12	  0.04	  0.06	  0.08	  0.05	
Decode	  0.06	  0.08	  1.21x		  0.08	  0.04	  0.13	  0.87	

Message size = 200
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  0.44	  0.48	  1.08x		  0.83	  0.29	  0.39	  0.69	  0.38	
Decode	  0.48	  0.67	  1.41x		  0.70	  0.29	  1.21	  8.93	

Message size = 2000
	modpb64	apache	improvement	modpb85	modpurl	modpb16	modpb2	modpjs
Encode	  4.06	  4.13	  1.02x		  7.91	  2.76	  3.68	  6.80	  3.67	
Decode	  4.70	  6.42	  1.37x		  6.80	  2.75	 11.97	 90.49
```

## 1.83 Ghz Intel Core Duo, gcc 4.0.1, -O3 ##

(aka Mac-Mini).  The "Core Duo" CPU is now obsolete (replaced by the "Core 2 Duo").

```
$ date
Tue Jun  5 09:45:07 EDT 2007
$ ./speedtest

Message size = 20
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.09    0.12    1.33x           0.18    0.06    0.07    0.09    0.07
Decode    0.09    0.10    1.11x           0.11    0.06    0.18    1.02

Message size = 200
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.55    0.64    1.16x           1.41    0.45    0.49    0.84    0.57
Decode    0.63    0.79    1.25x           0.99    0.56    1.62   10.33

Message size = 2000
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    5.08    5.73    1.13x          13.66    4.40    4.66    8.33    5.48
Decode    6.11    7.42    1.21x           9.72    5.48   15.98  103.96
```

## 1.33Ghz PowerPC G4, gcc 4.0.1, -O3 ##

(aka, my old mac powerbook)

```
$ date
Tue Jun  5 09:53:47 EDT 2007
$ ./speedtest

Message size = 20
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.12    0.18    1.50x           0.17    0.13    0.13    0.20    0.18
Decode    0.14    0.23    1.64x           0.15    0.16    0.16    1.02

Message size = 200
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    0.84    1.59    1.89x           1.26    0.95    0.93    1.69    1.55
Decode    0.81    2.09    2.58x           1.16    1.25    1.18   10.07

Message size = 2000
        modpb64 apache  improvement     modpb85 modpurl modpb16 modpb2  modpjs
Encode    8.00   15.37    1.92x          12.13    9.09    9.04   16.69   15.43
Decode    7.69   20.45    2.66x          11.35   12.10   11.98  100.85

$ uname -a
Darwin nickg-pbook.local 8.9.0 Darwin Kernel Version 8.9.0: Thu Feb 22 20:54:07 PST 2007; root:xnu-792.17.14~1/RELEASE_PPC Power Macintosh powerpc
$ gcc --version
powerpc-apple-darwin8-gcc-4.0.1 (GCC) 4.0.1 (Apple Computer, Inc. build 5363)
```