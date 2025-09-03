# Quantic Dream Engine Sources

Source code for Quantic Dream's in-house engine. This copy is combined from copies dated from April and July of 2006, which were obtained and sorted from an earlier leak from Sony's private developer forum.

This is available here for the sake of preservation, research and nothing more. Assets are not included (not that they were even available).

**Mind that the source files seem to use ISO-8859-1 encoding, so be sure to configure that in your editor if you don't want to mess things up!**

## Further Observations

It appears that QD started developing this engine in the late 2000s (November perhaps), so it doesn't appear to have any ancestory with the technology behind Omikron, at least not directly.
It's likely this engine was originally developed and first used alongside Fahrenheit.

As a mantter of fact, there *are* two references to Fahrenheit here.
One under [StreamHandleBlendShapeAnim.cpp](3dengine/Sources/Behavior/Datas/StreamableBlendShapeAnim/StreamHandleBlendShapeAnim.cpp) and another under [RuntimeXBox.vcproj](runtime/RuntimeXBox.vcproj).

Another game that QD worked on prior to Fahrenheit, but never shipped, was [Quark](https://www.unseen64.net/2024/03/24/quark-dreamcast-cancelled/), but there don't seem to be any references to that game though it's not outside the realms of possibilities that game was using this technology too - however this seems to post-date it (Quark was revealed in August of 2000, but this technology seems to have started development in November 2000).

That said, there *is* a very minor reference to Nomad Soul 2 under [RuntimeHeader.cpp](runtime/Sources/Application/RuntimeHeader.cpp).
