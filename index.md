---
layout: home
permalink: /
title: RestFrames
excerpt: Software library for HEP event analysis
buttons: true
image:
  feature: blue_jigsaw.jpg
---

<section>
<div class="tiles">

<div class="tile">
  <h2 class="post-title">Choose a decay tree</h2>
  <p class="post-excerpt">Create a fully configurable decay tree to
  analyze HEP events, designed for the processes you want to
  study. Include all the decay tree details or use a useful
  abstraction with only the features you are interested in.</p>
</div><!-- /.tile -->

<div class="tile">
  <h2 class="post-title">Derive a basis</h2>
  <p class="post-excerpt">Each decay tree implicitly defines a
  complete basis of kinematic observables. Use masses, momenta, and
  decay angles, evaluated in any reference frame, to extract the most
  information from events.</p>
</div><!-- /.tile -->

<div class="tile">
  <h2 class="post-title">Jigsaws under the hood</h2>
  <p class="post-excerpt">Whether your events contain combinatoric
  ambiguites due to identical final state particles or missing
  information resulting from one or more invisible particles,
  Recursive Jigsaw Reconstruction has you covered.</p>
</div><!-- /.tile -->

<div class="tile">
  <h2 class="post-title">Analyze anything</h2>
  <p class="post-excerpt">Decay trees can be of arbitrary complexity
  with as many reconstructed or invisible particles as you like. The
  only limitation is the number of particles you reconstruct in your
  event and your imagination in choosing Jigsaws.</p>
  </div><!-- /.tile -->

</div><!-- /.tiles -->
</section>

---
 
<p><h2>RestFrames news</h2></p>

<div class="tiles">
  {% for post in site.categories.news limit:4 %}
    {% include post-grid.html %}
  {% endfor %}
</div><!-- /.tiles -->

---

<p><h2>Recently added examples</h2></p>

<div class="tiles">
  {% for post in site.categories.examples limit:4 %}
    {% include example-grid.html %}
  {% endfor %}
</div><!-- /.tiles -->

---
