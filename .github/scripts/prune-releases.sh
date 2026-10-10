#!/usr/bin/env bash
set -euo pipefail

: "${GH_REPO:?GH_REPO must identify the release repository}"

# Finish pagination before deleting anything: deletion changes page boundaries.
releases=$(gh api "repos/$GH_REPO/releases?per_page=100" --paginate --slurp)
older_ids=$(jq -r '
  [.[][] | select(.draft == false and .published_at != null)]
  | if length == 0 then error("No published release to retain")
    else
      sort_by(.published_at, .id) | reverse | .[1:]
      | map(.id | tostring) | join("\n")
    end
' <<< "$releases")

# Choose by publication time, not this run's tag: an old run may be retried.
# Deleting a release removes its assets but preserves its Git tag.
while IFS= read -r release_id; do
  [[ -n "$release_id" ]] || continue
  echo "Deleting older release $release_id"
  gh api --method DELETE "repos/$GH_REPO/releases/$release_id"
done <<< "$older_ids"
