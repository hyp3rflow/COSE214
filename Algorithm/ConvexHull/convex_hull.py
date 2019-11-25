# Academic English II convex-hull presentation
# https://github.com/hyp3rflow
# hyperflow@korea.ac.kr 

import matplotlib.pyplot as plt
import random
import math


def main():
    max_number = int(input())

    points = []

    for _ in range(max_number):
        points.append((random.randint(0, 100), random.randint(0, 100)))

    plt.figure(num=None, figsize=(10, 10), dpi=100)
    ax = plt.axes()
    ax.set_facecolor('black')

    plt.title("Let's make the convex-hull")
    plt.scatter([i[0] for i in points], [i[1] for i in points], c='white')
    plt.ion()
    plt.waitforbuttonpress()

    convex_hull(points)
    plt.waitforbuttonpress()


def convex_hull(points):
    sort_by_x = sorted(points, key=lambda x: x[0])

    p1 = sort_by_x[0]
    pn = sort_by_x[-1]
    plt.scatter(p1[0], p1[1], c='r')
    plt.scatter(pn[0], pn[1], c='r')
    plt.waitforbuttonpress()

    s1, s2 = separate_points(points, p1, pn)
    convex_hull_main(s1, p1, pn)
    convex_hull_main(s2, pn, p1)


def convex_hull_main(sets, from_point, to_point):
    if len(sets) == 0:
        plt.plot((from_point[0], to_point[0]),
                 (from_point[1], to_point[1]), color='yellow')
        plt.waitforbuttonpress()
        return

    plt.scatter((from_point[0], to_point[0]),
                (from_point[1], to_point[1]), c='r')
    plt.waitforbuttonpress()
    
    plt.plot((from_point[0], to_point[0]),
                (from_point[1], to_point[1]), c='gray')
    plt.waitforbuttonpress()

    a = to_point[1] - from_point[1]
    b = from_point[0] - to_point[0]
    c = to_point[1] * from_point[0] - to_point[0] * from_point[1]

    dist = [distance(a, b, c, i) for i in sets]
    pmax = dist.index(max(dist))

    plt.scatter(sets[pmax][0], sets[pmax][1], c='g')
    plt.waitforbuttonpress()

    s1, _ = separate_points(sets, from_point, sets[pmax])
    s2, _ = separate_points(sets, sets[pmax], to_point)

    convex_hull_main(s1, from_point, sets[pmax])
    convex_hull_main(s2, sets[pmax], to_point)


def distance(a, b, c, point):
    return abs((a * point[0] + b * point[1] - c) / math.sqrt(a*a + b*b))


def separate_points(points, from_point, to_point):
    a = to_point[1] - from_point[1]
    b = from_point[0] - to_point[0]
    c = to_point[1] * from_point[0] - to_point[0] * from_point[1]

    s1 = []
    s2 = []

    for point in points:
        cur_x = point[0]
        cur_y = point[1]

        chksign = cur_x * a + b * cur_y - c

        if chksign < 0:
            s1.append(point)
        elif chksign > 0:
            s2.append(point)

    return s1, s2


if __name__ == '__main__':
    main()
