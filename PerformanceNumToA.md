## Intel Core i7 @ 2.66Ghz ##

Apple OS 10.7, gcc 4.2, Intel Core i7
```
    type   	sprintf	snprf	numtoa	improvement
unsigned 8	185005	151151	7075	21.4x
unsigned 16	172275	161701	12587	12.8x
unsigned 32	173841	169550	20584	8.2x
signed 8	154896	174714	8139	21.5x
signed 16	172830	179046	14794	12.1x
signed 32	169077	160026	20709	7.7x
unsigned 32 hex	143673	163338	5836	28.0x

    type   	%e	%f	%g	dtoa	improvement
double		310011	319852	251635	53777	4.7x
```
## Intel Xeon CPU L5310  @ 1.60GHz ##


Linux, gcc 3.4.6, -O3

```
$ date
Thu Jul  5 16:57:59 UTC 2007
$./speedtest_numtoa
    type        sprintf snprf   numtoa  improvement
unsigned 8      200000  210000  20000   10.5x
unsigned 16     220000  230000  40000   5.8x
unsigned 32     220000  230000  50000   4.6x
signed 8        200000  210000  20000   10.5x
signed 16       230000  220000  50000   4.4x
signed 32       220000  220000  60000   3.7x

    type        %e      %f      %g      dtoa    improvement
double          1610000 2280000 1590000 120000  13.2x

```


## AMD 2.2G Dual Core Operton Processor 275 ##

With gcc 3.4.4, -O3

```
$ date
Tue Apr 10 02:10:26 UTC 2007

$ ./speedtest_numtoa 
    type        sprintf snprf   numtoa  improvement
unsigned 8      200000  210000  10000   21.0x
unsigned 16     190000  190000  30000   6.3x
unsigned 32     160000  190000  30000   6.3x
signed 8        190000  210000  20000   10.5x
signed 16       180000  210000  30000   7.0x
signed 32       170000  190000  40000   4.8x
double          1670000 1590000 110000  14.5x
```


## Intel 2.2 Ghz Core 2 Duo ##

I forgot what machine this was.  I think it was MacBookPro, running 10.4.X

With gcc 4.0.1, -O3

```
~/stringencoders-v3.1.0 nickg$ date
Fri May 11 17:23:04 EDT 2007

~/stringencoders-v3.1.0 nickg$ ./speedtest_numtoa 
    type        sprintf snprf   numtoa  improvement
unsigned 8      19      18      2       9.0x
unsigned 16     20      19      3       6.3x
unsigned 32     19      19      3       6.3x
signed 8        18      20      1       20.0x
signed 16       20      21      2       10.5x
signed 32       19      20      3       6.7x

    type        %e      %f      %g      dtoa    impro
double          54      61      43      13      3.3x
```

## Intel 2.2 Ghz Core 2 Duo, Mac OS X 10.5 ##

New MacBook, Running Mac OS X 10.5.1.  I believe this has a different motherboard than the version before.

```
$ date
Thu Dec  6 09:50:17 EST 2007
./speedtest_numtoa
    type   	sprintf	snprf	numtoa	improvement
unsigned 8	193380	183893	15041	12.2x
unsigned 16	204912	203507	23012	8.8x
unsigned 32	206285	202143	26785	7.5x
signed 8	187765	189751	16240	11.7x
signed 16	214287	209203	26885	7.8x
signed 32	201955	205160	32701	6.3x

    type   	%e	%f	%g	dtoa	improvement
double		541438	593712	434464	84678	5.1x
```