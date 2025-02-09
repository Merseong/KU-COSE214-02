#! /usr/bin/env Rscript
png("convex.png", width=700, height=700)
plot(1:100, 1:100, type="n")

# 100 points
points(70, 16)
points(65, 81)
points(63, 5)
points(81, 71)
points(77, 24)
points(74, 90)
points(34, 40)
points(61, 76)
points(56, 54)
points(83, 23)
points(56, 37)
points(100, 41)
points(95, 24)
points(4, 11)
points(78, 68)
points(85, 47)
points(83, 1)
points(80, 45)
points(57, 60)
points(15, 33)
points(36, 40)
points(75, 69)
points(32, 87)
points(96, 39)
points(41, 30)
points(61, 96)
points(66, 12)
points(88, 12)
points(35, 44)
points(22, 13)
points(63, 58)
points(11, 46)
points(10, 90)
points(42, 66)
points(2, 9)
points(51, 89)
points(100, 25)
points(57, 31)
points(11, 4)
points(70, 3)
points(33, 82)
points(50, 50)
points(94, 90)
points(61, 80)
points(33, 82)
points(44, 95)
points(92, 55)
points(92, 1)
points(96, 34)
points(19, 97)
points(94, 69)
points(37, 93)
points(45, 93)
points(24, 7)
points(96, 93)
points(10, 80)
points(74, 59)
points(29, 19)
points(48, 89)
points(99, 32)
points(23, 94)
points(27, 14)
points(48, 18)
points(66, 96)
points(3, 84)
points(44, 96)
points(4, 81)
points(89, 48)
points(25, 12)
points(55, 73)
points(56, 64)
points(52, 81)
points(74, 81)
points(100, 22)
points(69, 50)
points(53, 43)
points(43, 31)
points(8, 43)
points(1, 74)
points(38, 55)
points(9, 33)
points(3, 13)
points(13, 91)
points(12, 38)
points(54, 66)
points(10, 61)
points(81, 61)
points(41, 55)
points(93, 92)
points(28, 62)
points(93, 32)
points(56, 88)
points(15, 64)
points(30, 67)
points(37, 19)
points(21, 97)
points(51, 23)
points(61, 16)
points(65, 73)
points(53, 70)

# 14 line segments
segments(100, 41, 100, 25)
segments(100, 41, 96, 93)
segments(83, 1, 11, 4)
segments(83, 1, 92, 1)
segments(10, 90, 19, 97)
segments(10, 90, 3, 84)
segments(2, 9, 11, 4)
segments(2, 9, 1, 74)
segments(100, 25, 100, 22)
segments(92, 1, 100, 22)
segments(19, 97, 21, 97)
segments(96, 93, 66, 96)
segments(66, 96, 21, 97)
segments(3, 84, 1, 74)
dev.off()
