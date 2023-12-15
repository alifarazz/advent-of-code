const std = @import("std");

const line_maxlen = 1024;

const nums = [_][]const u8{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

fn solve_silver(line: []u8) u32 {
    var line_acc: u32 = 0;
    for (line) |c| {
        if (c <= '9' and c >= '0') {
            line_acc += c - '0';
            break;
        }
    }
    line_acc *= 10;
    for (0..line.len) |i_| {
        const i = line.len - i_ - 1;
        const c = line[i];
        if (c <= '9' and c >= '0') {
            line_acc += c - '0';
            break;
        }
    }
    return line_acc;
}

fn solve_gold(line: []u8) u32 {
    var line_acc: u32 = 0;
    var found: bool = false;
    for (0..line.len) |i| {
        if (found == true) break;

        for (1.., nums) |num, numstr| {
            if (i + numstr.len <= line.len and std.mem.startsWith(u8, numstr, line[i .. i + numstr.len])) {
                found = true;
                line_acc = @intCast(num);
            }
        }

        const c = line[i];
        if (c <= '9' and c >= '0') {
            line_acc = c - '0';
            break;
        }
    }

    line_acc *= 10;

    found = false;
    for (0..line.len) |i_| {
        const i = line.len - i_ - 1;
        if (found == true) break;

        for (1.., nums) |num, numstr| {
            if (i + numstr.len <= line.len and std.mem.startsWith(u8, numstr, line[i .. i + numstr.len])) {
                found = true;
                line_acc += @intCast(num);
            }
        }

        const c = line[i];
        if (c <= '9' and c >= '0') {
            line_acc += c - '0';
            found = true;
        }
    }
    return line_acc;
}

pub fn main() !void {
    const stdin = std.io.getStdIn().reader();
    const stdout = std.io.getStdOut().writer();

    var buf: [line_maxlen]u8 = undefined;
    var silver: u32 = 0;
    var gold: u32 = 0;
    while (try stdin.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        silver += solve_silver(line);
        gold += solve_gold(line);
    }
    try stdout.print("{d}\n{d}\n", .{ silver, gold });
}
