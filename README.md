# watchy999
A Multi watch-face solution for Watchy

What started out as a way to have more than one watch face on the Watchy by SQFMI, has turned into some-what of a firmware enhancement. I've chipped away at the base firmware enough to chisel out something that is more along the lines of what I want the Watchy to be, more like my Pebble. With that said, is this the best option for you? It depends, if you're an enthusaist, probably. If you're a seasoned developer, maaaybe? 

Why Watchy999?

In learning Arduino and creating a way for multiple watch faces, I've made what works best "for me." I've standardized basic time functions to make creating watch faces as easy as possible. By simply looking over my example watch faces, you should be able to discern what's necessary to build your own watch face with little to no experience coding. That's how I started, I knew nothing, and thanks to a few thoughtful devs on the Pebble, I found my niche. I hope this helps Watchy999 folks in that same way.

Offical Release or not?

No, this is not an official release, yet... I need to flesh out a proper Readme, and that's gonna take some time given how large this project has grown.

Config999.h

This is where your one-time configurations reside. Hopefully, all future updates will not touch config999, and therefore updating to the latest revision of wathcy999 will not require any further updates. Most everything required to run watchy999 is explained in config999.h. Edit config999 once and you can set it and forget it.

Advantges over the stock firmware

It's got my watch faces, do I even need to say more? JK!!! But yeah, I do make some premium watch faces, so yeah, this will be the only way to get them. So what are the advantages? Once you've configured config999.h:

• Persistent configurations - Whatever settings you've configured on your Watchy will persist even if your battery dies, or install other watch faces. 

• Multipile Wifi Access Points - Define multiple Wifi Access Points so you're always connected throughout your day.

• Mulple built-in premium watch faces - I'm a designer, so this isn't developer art. Conversely, my code looks like I wrote it in crayon, but you can't see it on the watch, so no harm no foul! ;)

• Color, Layout and Action buttons - Press a top or bottom right button on a watch face to see what happens. Colors may change, animations may trigger, or your layout will change.

• Configure everything from the watch - This is just how it should be right?

• Sleep mode - Want to really maximize your battery? Use sleep mode. Configure when to start sleep mode, or enable it on demand and easily add an extra day or two between charges.

##UPDATE

I have updated this with GuruSR's RTC library which should allow this to run on the new Watchys that have a different RTC (Real Time Clock). It's still a work in progress, and I do not have a new watch to test it on. Please read the intructions.txt file in the libraryFiles folder for more details.

To be continued...
