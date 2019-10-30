#! /usr/bin/env Rscript
png("convex.png", width=700, height=700)
plot(1:100.000, 1:100.000, type="n")

#points
points(17.591,44.294)
points(36.383,88.048)
points(48.530,46.031)

#line segments
segments(17.591,44.294,36.383,88.048)
segments(36.383,88.048,48.530,46.031)
segments(48.530,46.031,17.591,44.294)
dev.off()
