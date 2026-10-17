#!/usr/bin/env bash
set -euo pipefail

: "${GITHUB_SHA:?GITHUB_SHA must identify the release commit}"
: "${GH_REPO:?GH_REPO must identify the release repository}"

release_tag="release-$GITHUB_SHA"
short_sha="${GITHUB_SHA::7}"
shopt -s nullglob
release_assets=(dist/Lander-*.tar.gz dist/Lander-*.zip)
max_attempts=3

for ((attempt = 1; attempt <= max_attempts; attempt++)); do
  if gh release view "$release_tag" >/dev/null 2>&1; then
    if gh release upload "$release_tag" "${release_assets[@]}" --clobber; then
      exit 0
    fi
  elif gh release create "$release_tag" "${release_assets[@]}" \
    --target "$GITHUB_SHA" \
    --title "Release $short_sha" \
    --notes "Automated release build from the master branch."; then
    exit 0
  fi

  if ((attempt < max_attempts)); then
    echo "Release API request failed; retrying ($attempt/$max_attempts)." >&2
    sleep "$((attempt * 5))"
  fi
done

echo "Failed to publish release after $max_attempts attempts." >&2
exit 1
