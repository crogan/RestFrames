---
layout: home
title: "Examples"
date: 2016-05-22
modified: 2016-05-22
excerpt: "A collection of decay trees and analysis code to get you started with RestFrames"
tags: []
buttons: false
image:
  feature: examples_jigsaw.jpg

---

<div class="tiles">
{% for post in site.categories.examples reversed %}
  {% include example-grid.html %}
{% endfor %}
</div><!-- /.tiles -->
