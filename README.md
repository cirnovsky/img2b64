# img2b64

A lightweight program to transform images to Base64 codes. For blogging use.

## Usage

- `img2b64`: The compiled executable. `img2b64 [file1] [file2] ...` after which it opens the output file in VSCode. Just copy the content to the blog post. It's Markdown by default.

- `_img.sh`: `_img.sh [path]` searches all image files under `[path]` non-recursively and pass to `img2b64`.

- `ln.sh`: Run `ln.sh [username]` to create symlink under `/home/[username]/bin` as `_img`.
