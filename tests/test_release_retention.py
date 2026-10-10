"""Exercise release cleanup without contacting GitHub or deleting real releases."""

import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / ".github/scripts/prune-releases.sh"


class ReleaseRetentionTests(unittest.TestCase):
    def run_cleanup(self, pages, failure=""):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "releases.json").write_text(json.dumps(pages))
            (root / "calls").touch()
            mock = root / "gh"
            mock.write_text("""#!/usr/bin/env python3
import json, os, pathlib, sys
root = pathlib.Path(os.environ['MOCK_ROOT'])
args = sys.argv[1:]
with (root / 'calls').open('a') as log:
    log.write(json.dumps(args) + '\\n')
if args == ['api', 'repos/owner/repo/releases?per_page=100', '--paginate', '--slurp']:
    if os.environ['MOCK_FAILURE'] == 'list':
        print('[[{"id": 1}]]')  # Partial output must not trigger deletion.
        sys.exit(1)
    print((root / 'releases.json').read_text())
elif args[:3] == ['api', '--method', 'DELETE']:
    sys.exit(1 if os.environ['MOCK_FAILURE'] == 'delete' else 0)
else:
    sys.exit('Unexpected gh invocation: ' + repr(args))
""")
            mock.chmod(0o755)
            result = subprocess.run(
                ["bash", str(SCRIPT)], capture_output=True, text=True,
                env={**os.environ, "PATH": f"{root}:{os.environ['PATH']}",
                     "GH_REPO": "owner/repo", "MOCK_ROOT": directory,
                     "MOCK_FAILURE": failure},
            )
            calls = [json.loads(line) for line in (root / "calls").read_text().splitlines()]
            deleted = [int(call[-1].rsplit("/", 1)[1])
                       for call in calls if call[:3] == ["api", "--method", "DELETE"]]
            return result, deleted

    @staticmethod
    def release(identifier, published="2026-10-01T12:00:00Z", draft=False):
        return {"id": identifier, "published_at": published, "draft": draft}

    def test_first_release_is_kept(self):
        result, deleted = self.run_cleanup([[self.release(1)]])
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(deleted, [])

    def test_keeps_newest_publication_not_list_order_or_id(self):
        result, deleted = self.run_cleanup([[
            self.release(99), self.release(1, "2026-10-10T12:00:00Z"),
            self.release(5), self.release(100, None, draft=True),
        ]])
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(deleted, [99, 5])

    def test_pagination_and_tied_timestamps(self):
        releases = [self.release(i) for i in range(1, 206)]
        result, deleted = self.run_cleanup([releases[:100], releases[100:200], releases[200:]])
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(deleted, list(range(204, 0, -1)))

    def test_no_published_release_fails_without_deletion(self):
        for pages in ([[]], [[self.release(1, None, draft=True)]]):
            result, deleted = self.run_cleanup(pages)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(deleted, [])

    def test_listing_failure_never_deletes(self):
        result, deleted = self.run_cleanup([[self.release(1), self.release(2)]], "list")
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(deleted, [])

    def test_delete_failure_stops_cleanup(self):
        result, deleted = self.run_cleanup([[self.release(i) for i in range(1, 4)]], "delete")
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(deleted, [2])


if __name__ == "__main__":
    unittest.main()
