---
layout: post
title: Moved from Tumblr to Jekyll+GitHub
disqus_comments: true
---

As many other people running their blogs on [Tumblr](http://www.tumblr.com/) I've become painfully aware of their failures to provide an acceptable service. Hours and sometimes days of downtime is unacceptable, even for a free service, when one of the most important parts of the service is availability.

So after some [inquiries through Twitter](http://search.twitter.com/search?q=to:rsms+since:2011-01-20+until:2011-01-21+github+OR+posterous+OR+squarespace+OR+wordpress+OR+tumblr+OR+soup+OR+jekyll+OR+blogging+OR+blog+OR+gitblog) I finally decided to go for [Jekyll](https://github.com/mojombo/jekyll) and [GitHub Pages](http://pages.github.com/) (thanks to [@al3x](https://twitter.com/al3x), [@masolino](https://twitter.com/masolino) and everyone else who helped with answers in the right direction). As I'm already a paying subscriber to the fantastic GitHub I get the pages feature for free. Another thing I'm concerned with is peering to non-US countries, which is provided by GitHub as they sit on top of the very good Rackspace connections. As a comparison, fetching a simple HTML page from GitHub pages to Stockholm, Sweden takes about 250ms while fetching the same HTML from Tumblr can take up to 4 seconds to arrive.

I order to get all my data over from Tumblr I wrote a small script which queries the Tumblr API and converts them into Jekyll posts. The script is available at [_scripts/import-from-tumblr.js](https://github.com/rsms/rsms.github.com/blob/master/_scripts/import-from-tumblr.js) and is accompanied by [a script to generate a URL map](https://github.com/rsms/rsms.github.com/blob/master/_scripts/gen-tumblr-url-map.js), used to provide some sort of old links redirection. I'm using a simple javascript solution found in my [404.html](https://github.com/rsms/rsms.github.com/blob/master/404.html) but you might as well do something fancier with the generated url map.

So let's see how this turns out.
