const std = @import("std");
const builtin = @import("builtin");
const assert = std.debug.assert;

const zig_version = std.SemanticVersion{
    .major = 0,
    .minor = 14,
    .patch = 1,
};
comptime {
    // Compare versions while allowing different pre/patch metadata.
    const zig_version_eq = zig_version.major == builtin.zig_version.major and
        zig_version.minor == builtin.zig_version.minor and
        zig_version.patch == builtin.zig_version.patch;
    if (!zig_version_eq) {
        @compileError(std.fmt.comptimePrint(
            "unsupported zig version: expected {}, found {}",
            .{ zig_version, builtin.zig_version },
        ));
    }
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const default_c_flags = [_][]const u8{"-std=c99"};

    const connector_exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    connector_exe_mod.addCSourceFile(.{
        .file = b.path("src/connector.c"),
        .flags = &default_c_flags,
        .language = .c,
    });
    const connector_exe = b.addExecutable(.{
        .name = "tcp_connector",
        .root_module = connector_exe_mod,
    });

    const listener_exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    listener_exe_mod.addCSourceFile(.{
        .file = b.path("src/listener.c"),
        .flags = &default_c_flags,
        .language = .c,
    });
    const listener_exe = b.addExecutable(.{
        .name = "tcp_listener",
        .root_module = listener_exe_mod,
    });

    b.installArtifact(connector_exe);
    b.installArtifact(listener_exe);
}
