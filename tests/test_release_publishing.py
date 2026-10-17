"""Exercise release publishing retries without contacting GitHub."""

import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / ".github/scripts/publish-release.sh"


class ReleasePublishingTests(unittest.TestCase):
    def run_publish(self, create_failures=0):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "dist").mkdir()
            (root / "dist/Lander-linux-x86_64.tar.gz").touch()
            (root / "calls").touch()
            mock = root / "gh"
            mock.write_text("""#!/usr/bin/env python3
import json, os, pathlib, sys
root = pathlib.Path(os.environ['MOCK_ROOT'])
args = sys.argv[1:]
with (root / 'calls').open('a') as log:
    log.write(json.dumps(args) + '\\n')
if args[:2] == ['release', 'view']:
    sys.exit(0 if (root / 'created').exists() else 1)
if args[:2] == ['release', 'create']:
    attempts = int((root / 'create-attempts').read_text()) + 1 if (root / 'create-attempts').exists() else 1
    (root / 'create-attempts').write_text(str(attempts))
    if attempts <= int(os.environ['CREATE_FAILURES']):
        sys.exit(1)
    (root / 'created').touch()
    sys.exit(0)
if args[:2] == ['release', 'upload']:
    sys.exit(0)
sys.exit('Unexpected gh invocation: ' + repr(args))
""")
            mock.chmod(0o755)
            sleep = root / "sleep"
            sleep.write_text("#!/bin/sh\nexit 0\n")
            sleep.chmod(0o755)
            result = subprocess.run(
                ["bash", str(SCRIPT)],
                cwd=root,
                capture_output=True,
                text=True,
                env={
                    **os.environ,
                    "PATH": f"{root}:{os.environ['PATH']}",
                    "GITHUB_SHA": "0123456789abcdef",
                    "GH_REPO": "owner/repo",
                    "MOCK_ROOT": directory,
                    "CREATE_FAILURES": str(create_failures),
                },
            )
            calls = [json.loads(line) for line in (root / "calls").read_text().splitlines()]
            attempts = sum(call[:2] == ["release", "create"] for call in calls)
            return result, attempts

    def test_retries_transient_create_failure(self):
        result, attempts = self.run_publish(create_failures=1)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(attempts, 2)

    def test_stops_after_retry_limit(self):
        result, attempts = self.run_publish(create_failures=10)
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(attempts, 3)


if __name__ == "__main__":
    unittest.main()
