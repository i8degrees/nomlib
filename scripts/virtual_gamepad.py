#!/usr/bin/env python3
"""Harness for verifying nom_gamecontroller_ev input handling.

Uses evdev/UInput to create a virtual Logitech F310 game controller and
exercises the example's pause/resume, button, axis, and d-pad code paths.

Can be run standalone:
    scripts/virtual_gamepad.py [path/to/nom_gamecontroller_ev]

Or imported from a test runner:
    from scripts.virtual_gamepad import run_test
    rc, output = run_test('build/debug-dynamic/bin/nom_gamecontroller_ev')

Requires python-evdev and root or membership in the 'input' group.
"""
import os
import signal
import subprocess
import sys
import time

import evdev


def find_real_f310():
    """Return the first real Logitech F310 input device, or None."""
    for path in evdev.list_devices():
        try:
            d = evdev.InputDevice(path)
            if d.info.vendor == 0x046D and d.info.product == 0xC21D:
                return d
        except Exception:
            pass
    return None


# Mapping from Linux input event key codes to SDL GameController button indices
# for the Logitech F310 mapping used by SDL.
PHYSICAL_TO_SDL_BUTTON = {
    evdev.ecodes.BTN_A: 0,
    evdev.ecodes.BTN_B: 1,
    evdev.ecodes.BTN_X: 2,
    evdev.ecodes.BTN_Y: 3,
    evdev.ecodes.BTN_TL: 4,
    evdev.ecodes.BTN_TR: 5,
    evdev.ecodes.BTN_SELECT: 6,
    evdev.ecodes.BTN_START: 7,
    evdev.ecodes.BTN_MODE: 8,
    evdev.ecodes.BTN_THUMBL: 9,
    evdev.ecodes.BTN_THUMBR: 10,
}


def run_test(executable_path, grab_real=True, verbose=True):
    """Run the gamepad input test against the given example executable.

    Returns tuple (returncode, log_output).  returncode is 0 on success.
    """
    executable_path = os.path.abspath(executable_path)
    cwd = os.path.dirname(executable_path)
    cmd = [os.path.basename(executable_path)]
    log_lines = []

    env = os.environ.copy()
    if verbose:
        env["NOM_GC_VERBOSE"] = "1"

    # Help the example find its vcpkg debug libraries when launched from a
    # different interpreter/environment.
    vcpkg_debug_lib = os.path.join(cwd, "..", "vcpkg_installed",
                                   "x64-linux-dynamic", "debug", "lib")
    vcpkg_debug_lib = os.path.abspath(vcpkg_debug_lib)
    env["LD_LIBRARY_PATH"] = vcpkg_debug_lib + os.pathsep + env.get("LD_LIBRARY_PATH", "")

    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
        env=env,
    )

    real = find_real_f310()

    def cleanup(signum, frame):
        if proc.poll() is None:
            proc.terminate()
        try:
            if real is not None:
                real.ungrab()
        except Exception:
            pass
        sys.exit(0)

    signal.signal(signal.SIGTERM, cleanup)
    signal.signal(signal.SIGINT, cleanup)

    import threading

    def reader():
        for line in proc.stdout:
            log_lines.append(line)
            sys.stdout.write(line)

    t = threading.Thread(target=reader, daemon=True)
    t.start()

    def wait_for_logs(markers, timeout=10):
        start = time.time()
        text = ""
        while time.time() - start < timeout:
            text = "".join(log_lines)
            if all(m in text for m in markers):
                return True
            time.sleep(0.1)
        return False

    # Wait until the example has finished its own initialization so any
    # already-connected controller is already active.
    if not wait_for_logs(["Initialized game controller bindings for"]):
        print("[harness] timed out waiting for example initialization")

    # Exclude the real controller's input events from the test so the only
    # events the example receives are from our virtual device.
    if grab_real and real is not None:
        print(f"[harness] grabbing real controller at {real.path}")
        real.grab()
    elif grab_real:
        print("[harness] no real F310 found; proceeding without grab")

    # Create the virtual clone after the example is running so SDL adds it via
    # hotplug, keeping it separate from any real controllers.
    keys = set(PHYSICAL_TO_SDL_BUTTON.keys())
    abs_map = {
        evdev.ecodes.ABS_X: evdev.AbsInfo(-32768, 32767, 0, 16, 128, 0),
        evdev.ecodes.ABS_Y: evdev.AbsInfo(-32768, 32767, 0, 16, 128, 0),
        evdev.ecodes.ABS_RX: evdev.AbsInfo(-32768, 32767, 0, 16, 128, 0),
        evdev.ecodes.ABS_RY: evdev.AbsInfo(-32768, 32767, 0, 16, 128, 0),
        evdev.ecodes.ABS_Z: evdev.AbsInfo(0, 255, 0, 0, 0, 0),
        evdev.ecodes.ABS_RZ: evdev.AbsInfo(0, 255, 0, 0, 0, 0),
        evdev.ecodes.ABS_HAT0X: evdev.AbsInfo(-1, 1, 0, 0, 0, 0),
        evdev.ecodes.ABS_HAT0Y: evdev.AbsInfo(-1, 1, 0, 0, 0, 0),
    }
    ui = evdev.UInput(
        {evdev.ecodes.EV_KEY: keys, evdev.ecodes.EV_ABS: abs_map},
        name="Logitech Gamepad F310",
        vendor=0x046D,
        product=0xC21D,
        version=0x4014,
        bustype=0x0003,
    )
    print(f"[harness] created virtual device at {ui.devnode}")

    if not wait_for_logs(["Initialized game controller bindings for Logitech Gamepad F310 (ID: 0 )"]):
        print("[harness] timed out waiting for virtual controller initialization")

    def emit_button(physical_button):
        ui.write(evdev.ecodes.EV_KEY, physical_button, 1)
        ui.syn()
        time.sleep(0.05)
        ui.write(evdev.ecodes.EV_KEY, physical_button, 0)
        ui.syn()

    def emit_axis(axis, value):
        ui.write(evdev.ecodes.EV_ABS, axis, value)
        ui.syn()

    def emit_hat(x, y):
        emit_axis(evdev.ecodes.ABS_HAT0X, x)
        emit_axis(evdev.ecodes.ABS_HAT0Y, y)

    # Wait for the new PlayingState context to become active.
    time.sleep(0.5)

    # --- Pause / resume ---
    print("[harness] pressing START to pause")
    emit_button(evdev.ecodes.BTN_START)
    time.sleep(1.0)

    print("[harness] pressing START to resume")
    emit_button(evdev.ecodes.BTN_START)
    time.sleep(1.0)

    # --- All other buttons ---
    print("[harness] pressing each face/shoulder button")
    for physical_button in sorted(PHYSICAL_TO_SDL_BUTTON.keys()):
        if physical_button == evdev.ecodes.BTN_START:
            continue
        emit_button(physical_button)
        time.sleep(0.05)
    time.sleep(1.0)

    # --- Axes ---
    print("[harness] exercising analog axes")
    emit_axis(evdev.ecodes.ABS_X, 32767)
    emit_axis(evdev.ecodes.ABS_Y, -32768)
    emit_axis(evdev.ecodes.ABS_RX, -32768)
    emit_axis(evdev.ecodes.ABS_RY, 32767)
    emit_axis(evdev.ecodes.ABS_Z, 255)
    emit_axis(evdev.ecodes.ABS_RZ, 255)
    time.sleep(0.5)

    # Center sticks and release triggers.
    emit_axis(evdev.ecodes.ABS_X, 0)
    emit_axis(evdev.ecodes.ABS_Y, 0)
    emit_axis(evdev.ecodes.ABS_RX, 0)
    emit_axis(evdev.ecodes.ABS_RY, 0)
    emit_axis(evdev.ecodes.ABS_Z, 0)
    emit_axis(evdev.ecodes.ABS_RZ, 0)
    time.sleep(0.5)

    # --- D-pad ---
    print("[harness] exercising d-pad")
    emit_hat(1, 0)
    emit_hat(0, 1)
    emit_hat(-1, 0)
    emit_hat(0, -1)
    emit_hat(0, 0)
    time.sleep(0.5)

    # Clean up.
    ui.close()
    if real is not None:
        real.ungrab()
    if proc.poll() is None:
        proc.terminate()

    t.join(timeout=2)
    proc.stdout.close()

    output = "".join(log_lines)

    failures = []

    if "Paused" not in output or "Resumed from pause" not in output:
        failures.append("pause/resume markers missing")

    if "ERROR" in output:
        failures.append("ERROR output detected")

    expected_buttons = set(PHYSICAL_TO_SDL_BUTTON.values())
    expected_axes = {0, 1, 2, 3, 4, 5}
    seen_buttons = set()
    seen_axes = set()
    for line in output.splitlines():
        if "GC button event" in line:
            try:
                button = int(line.split()[-1])
                seen_buttons.add(button)
            except ValueError:
                pass
        elif "GC axis event" in line:
            parts = line.split()
            try:
                axis = int(parts[-2])
                seen_axes.add(axis)
            except (ValueError, IndexError):
                pass

    missing_buttons = expected_buttons - seen_buttons
    missing_axes = expected_axes - seen_axes

    if missing_buttons:
        failures.append(f"missing buttons: {sorted(missing_buttons)}")
    if missing_axes:
        failures.append(f"missing axes: {sorted(missing_axes)}")

    if failures:
        print("[harness] FAIL:", "; ".join(failures))
        print(f"[harness] seen buttons={sorted(seen_buttons)} axes={sorted(seen_axes)}")
        return 1, output

    print("[harness] PASS: pause/resume, all buttons, and all axes exercised.")
    return 0, output


def main():
    default_exe = "build/debug-dynamic/bin/nom_gamecontroller_ev"
    exe = sys.argv[1] if len(sys.argv) > 1 else default_exe
    return run_test(exe)[0]


if __name__ == "__main__":
    rc = main()
    sys.exit(rc)
