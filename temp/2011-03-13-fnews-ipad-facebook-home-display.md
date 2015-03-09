---
layout: post
title: An expensive photo frame
tags: apple, ipad, facebook
fb_comments: 25
---

This weekend's hack is an iPad app which displays your Facebook news feed in a "photo frame" fashion — large text, automatic, simplistic and suitable for passive viewing.

[![Photo](http://farm6.static.flickr.com/5017/5522785997_b9e2ff887d_z.jpg)](http://rsms.me/projects/fnews/)

I never really got the iPad (my laptop or smartphone is always around) and have probably used it for about a total of 10 hours, so better make good use of the thing.

The app is actually a web app. iOS web apps are perfect for these simple hacks as no super-mega performance is required and the hassle of getting a native app on the App Store would yield more work than actually building the app.

**Visit [rsms.me/projects/fnews](http://rsms.me/projects/fnews/)** in your iPad, add the app to your Home Screen and start it.

It's built on the [Facebook Graph API](https://developers.facebook.com/docs/reference/api/) which provides a smooth and low-barrier user experience.

## Technical hurdles

<img src="http://farm6.static.flickr.com/5258/5523503142_4af3978bce_z.jpg" align="right">The **streaming/real-time API does not currently support news feed**, thus a polling techinque is used instead. This works really well with a polling interval of around 1 minute.

**The "like" button** is unfortunately somewhat defunct as the only API for "like" is the Open Graph one which causes the *app* to like something *rather than the user*. **Update:** I've added a request for the [`publish_stream` permission](https://developers.facebook.com/docs/authentication/permissions/). If you already have connected Fnews with Facebook, you'll need to log out (tap the hard-to-see gear in the bottom-right corner) and then log in again to give Fnews the rights to "like" something on your behalf. I really wish there was a separate `publish_like` permission as `publish_stream` sounds really scary to most users and will probably lower the user count. Anyhow, *"Like" to the people!*.

**Foursquare check-ins** use maps from Google Maps (the Static Map API) rather than Foursquare as their (Foursquare's) API is too complex for this quick hack. Google Maps are prettier anyway. I never managed to "snapshot" any Gowalla check-ins during the weekend (they are relatively rare in my perspective), so currently no Gowalla "candy additions".

**Logging in to Facebook** using the [Facebook JavaScript API](https://developers.facebook.com/docs/reference/javascript/) should be avoided in "web apps" as it relies on `window.open`, which will cause a blank white screen to take over your app. This is a broken behavior of iOS "web apps" framework. Instead, when logging in to Facebook while in "web app" mode, Fnews simply sends you to Facebook, which eventually sends you back to Fnews (thought HTTP 302). It's not pretty but it works. Note that although the [`FB.ui`](https://developers.facebook.com/docs/reference/javascript/FB.ui/) API supports iframe dialogs, there's an exception for authentication dialogs which must be opened in separate windows.

**Instagram pictures** turned out to be rather simple to acquire as they provide a simpler ["embedding" API](http://instagram.com/developer/embedding/) — in addition to their full-blown OAuthed behemoth API — which allows to simply concatenate a regular link URL with "media" to build a higher-res image URL. Neat.

## Oh sweet, sweet source code

Available through GitHub: <https://github.com/rsms/rsms.github.com/tree/master/projects/fnews>
