<?php
if (PHP_OS === 'Windows') {
    exec(sprintf("rd /s /q %s", escapeshellarg($argv[1])));
} else {
    exec(sprintf("rm -rf %s", escapeshellarg($argv[1])));
}
